import os
import subprocess

devkitppc = os.environ.get('DEVKITPPC')
path_cc = os.path.join(devkitppc, "bin", "powerpc-eabi-gcc")
path_objcopy = os.path.join(devkitppc, "bin", "powerpc-eabi-objcopy")

extra_build_flags = []


def build():
    print("Building payload...")

    flags = ["-g", "-Os", "-fPIE", "-std=c++20", '-Wall', '-Werror', "-Wsuggest-override", "-n", "-fno-rtti",
              "-fno-exceptions", "-fno-sized-deallocation", "-ffunction-sections", "-fdata-sections", "-fshort-wchar",
              "-Wl,--gc-sections", "-Wno-address-of-packed-member"]
    flags += extra_build_flags

    out_path = os.path.join("build", "room")
    binary_path = os.path.join("binary", "payload.room.bin")
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

    build()
