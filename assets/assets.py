import os
import sys

def list_files(dir, hidden: bool=False) -> list[str]:
	ret: list[str] = []
	for file in os.listdir(dir):
		if file.startswith(".") and not hidden: continue
		path = os.path.join(dir, file)
		if os.path.isdir(path):
			sub = list_files(path, hidden)
			ret.extend((os.path.join(file, name) for name in sub))
		else:
			ret.append(file)
	return ret

def path_to_identifier(path: str) -> str:
	ret: str = ""
	allowed: str = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789_"
	for i in path:
		if i not in allowed:
			ret += "_"
		else:
			ret += i
	return ret

def generate_src(out, folder, prefix):
	ids: dict = {}
	for file in list_files(folder):
		id = prefix + "_" + path_to_identifier(file)
		if id in ids: raise KeyError(f"id {id} already exists")
		ids[id] = file
	lines: str = ""
	lines += "#include\"main/builtin.h\"\n"
	lines += ".section \".rodata\"\n"
	lines += "\n/* files body */\n"
	lines += "".join((f"DECL_BIN({id},\"{folder}/{ids[id]}\")\n" for id in ids))
	lines += "\n/* files name table */\n"
	lines += "".join((f"_name_{id}: .ascii \"{ids[id]}\\0\";.balign 8;\n" for id in ids))
	lines += ".balign 8;\n"
	lines += "\n/* files list */\n"
	lines += f"DECL_ASM_VAR({prefix}_file_list)\n"
	lines += "".join((
		f"/* file {ids[id]} */\n"+
		f"ASM_PTR_NUM _name_{id}\n"+
		f"ASM_PTR_NUM bin_{id}_start\n"+
		f".quad bin_{id}_end - bin_{id}_start\n"
	for id in ids))
	lines += "/* list end */\n"
	lines += "ASM_PTR_NUM 0\n"
	lines += "ASM_PTR_NUM 0\n"
	lines += ".quad 0\n"
	lines += f"DECL_ASM_END({prefix}_file_list)\n"
	with open(out, "w") as f:
		f.write(lines)

if __name__ == "__main__":
	if len(sys.argv) != 4:
		print(f"Usage: {sys.argv[0]} OUTPUT FOLDER PREFIX")
		sys.exit(1)
	generate_src(sys.argv[1], sys.argv[2], sys.argv[3])
