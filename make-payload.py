import os
import subprocess

devkitppc = os.environ.get('DEVKITPPC')
path_cc = os.path.join(devkitppc, "bin", "powerpc-eabi-gcc")
path_objcopy = os.path.join(devkitppc, "bin", "powerpc-eabi-objcopy")

languages = ["ALL", "PTBR", "RU"]


def build(_language):
    print("Building payload...")

    flags = ["-g", "-Os", "-fPIE", "-std=c++20", '-Wall', '-Werror', "-Wsuggest-override", "-n", "-fno-rtti",
             "-fno-exceptions", "-fno-sized-deallocation", "-ffunction-sections", "-fdata-sections", "-fshort-wchar",
             "-Wl,--gc-sections", "-Wno-address-of-packed-member", f"-DLANGUAGE={_language}"]

    out_path = os.path.join("build", "room")
    binary_path = os.path.join("binary", f"payload.{_language}.bin")
    subprocess.run([path_cc, "-o" + out_path + ".elf", "room.cpp", "-Tpayload.ld",
                    "-I.", "-Wl,--defsym=ORIGIN_ADDRESS=0x80001800"] + flags).check_returncode()
    subprocess.run([path_objcopy, out_path + ".elf", binary_path, "-O", "binary"]).check_returncode()


if __name__ == "__main__":
    try:
        os.mkdir("build")
    except:
        pass

    try:
        os.mkdir("binary")
    except:
        pass

    for language in languages:
        build(language)
