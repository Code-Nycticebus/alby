from pathlib import Path
import subprocess

examples_folder = Path("examples")
examples_folder.mkdir(exist_ok=True)

bytecode_folder = Path("bytecode")
bytecode_folder.mkdir(exist_ok=True)

for file in examples_folder.rglob("*.lsd"):
    return_code = subprocess.run([".build/debug/bin/alby", file, "-c", Path(bytecode_folder, file.with_suffix("").name)])
    if return_code.returncode:
        print(" ".join(map(str, return_code.args)))
        break;
