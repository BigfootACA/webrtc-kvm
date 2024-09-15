import os
import sys


def list_files(folder: str, hidden: bool = False) -> list[str]:
	ret: list[str] = []
	for file in os.listdir(folder):
		if file.startswith(".") and not hidden:
			continue
		path = os.path.join(folder, file)
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
		ret += i if i in allowed else "_"
	return ret


def generate_src(out: str, folder: str, prefix: str) -> None:
	idents: dict = {}
	for file in list_files(folder):
		ident = prefix + "_" + path_to_identifier(file)
		if ident in idents:
			raise KeyError(f"id {ident} already exists")
		idents[ident] = file
	lines: str = ""
	lines += "#include\"main/builtin.h\"\n"
	lines += ".section \".rodata\"\n"
	lines += "\n/* files body */\n"
	lines += "".join((f"DECL_BIN({ident},\"{folder}/{idents[ident]}\")\n" for ident in idents))
	lines += "\n/* files name table */\n"
	lines += "".join((f"_name_{ident}: .ascii \"{idents[ident]}\\0\";.balign 8;\n" for ident in idents))
	lines += ".balign 8;\n"
	lines += ".section \".data\"\n"
	lines += "\n/* files list */\n"
	lines += f"DECL_ASM_VAR({prefix}_file_list)\n"
	lines += "".join((
		f"/* file {idents[ident]} */\n" +
		f"ASM_PTR_NUM _name_{ident}\n" +
		f"ASM_PTR_NUM bin_{ident}_start\n" +
		f".quad bin_{ident}_end - bin_{ident}_start\n"
		for ident in idents))
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
