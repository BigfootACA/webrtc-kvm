import os
import sys
import shutil
import re
from elftools.elf.elffile import ELFFile
from elftools.elf.sections import SymbolTableSection,Symbol


def find_symbols(path: str, regex: str) -> list[str]:
	ret: list[str] = []
	pattern = re.compile(regex)
	with open(path, "rb") as f:
		elf = ELFFile(f)
		symtab = elf.get_section_by_name(".symtab")
		if not symtab:
			return []
		for symbol in symtab.iter_symbols():
			if re.search(pattern, symbol.name):
				ret.append(symbol.name)
	return ret

def copy_file(src: str, dst: str):
	if os.path.exists(dst):
		if os.path.isdir(dst):
			raise Exception(f"target {dst} is folder")
		os.remove(dst)
	if os.path.islink(src):
		tgt = os.readlink(src)
		os.symlink(tgt, dst)
	else:
		shutil.copy(src, dst)


def copy_folder(src: str, dst: str):
	if not os.path.exists(dst):
		os.mkdir(dst)
		shutil.copymode(src, dst)
	for item in os.listdir(src):
		src_path = os.path.join(src, item)
		dst_path = os.path.join(dst, item)
		if os.path.isdir(src_path):
			copy_folder(src_path, dst_path)
		else:
			copy_file(src_path, dst_path)


def do_copy_plugins(folder: str, plugins: list[str]):
	if not os.path.exists(folder):
		os.mkdir(folder, 0o755)
	folder = os.path.realpath(folder)
	links = []
	files = []
	exts = [".so", ".dll"]
	for plugin in plugins:
		if not any(plugin.endswith(ext) for ext in exts):
			raise Exception(f"bad plugin {plugin}")
		name = os.path.basename(plugin)
		path = os.path.join(folder, name)
		copy_file(plugin, path)
		if os.path.islink(path):
			links.append(path)
		else:
			files.append(path)
	for link in links:
		name = os.path.basename(link)
		target = os.path.realpath(link)
		if os.path.dirname(target) != folder:
			raise Exception(f"symlink {name} out of folder")
	for file in files:
		name: str = os.path.basename(file)
		dot = name.rfind(".")
		sep = name.find("_")
		if sep < 0 or dot < 0:
			raise Exception(f"bad plugin name {name}")
		plugin_ext = name[dot+1:]
		plugin_type = name[0:sep]
		plugin_class = name[sep+1:-3]
		start = f"webrtc_plugin_{plugin_type}_"
		symbols = find_symbols(file, f"^{start}")
		if len(symbols) <= 0:
			raise Exception(f"plugin {name} have no any exports")
		def create_new_name(symbol: str):
			new_name = f"{plugin_type}_{symbol}.{plugin_ext}"
			new_path = os.path.join(folder, new_name)
			if os.path.exists(new_path):
				if not os.path.islink(new_path):
					raise Exception(f"plugin {new_name} conflict with {file} for {symbol}")
				elif os.readlink(new_path) != name:
					raise Exception(f"plugin {new_name} have no any exports")
			else:
				os.symlink(name, new_path)
		def proc_get_symbol(symbol: str):
			if symbol != plugin_class:
				create_new_name(symbol)
		def proc_symbol(symbol: str):
			if symbol.startswith("get_"):
				proc_get_symbol(symbol[4:])
		for symbol in symbols:
			proc_symbol(symbol[len(start):])


def do_install_plugins(folder: str, install: str):
	dest = os.path.join(os.getenv("MESON_INSTALL_DESTDIR_PREFIX"), install)
	copy_folder(folder, dest)


if __name__ == "__main__":
	if len(sys.argv) <= 0:
		print(f"Usage: {sys.argv[0]} <ACTION> [ARGUMENTS...]")
		sys.exit(1)
	if sys.argv[1] == "copy-plugins":
		if len(sys.argv) <= 3:
			print(f"Usage: {sys.argv[0]} copy-plugins <FOLDER> <PLUGINS...>")
			sys.exit(1)
		do_copy_plugins(sys.argv[2], sys.argv[3:])
	elif sys.argv[1] == "install-plugins":
		if len(sys.argv) != 4:
			print(f"Usage: {sys.argv[0]} install-plugins <FOLDER> <INSTALL>")
			sys.exit(1)
		do_install_plugins(sys.argv[2], sys.argv[3])
	else:
		print(f"unknown action {sys.argv[1]}")
		sys.exit(1)
