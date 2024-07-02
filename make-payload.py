import io
import os
import subprocess

devkitppc = os.environ.get('DEVKITPPC')
path_cc = os.path.join(devkitppc, "bin", "powerpc-eabi-gcc")
path_objcopy = os.path.join(devkitppc, "bin", "powerpc-eabi-objcopy")
path_as = os.path.join(devkitppc, "bin", "powerpc-eabi-as")
path_bin2s = "bin2s"
path_cert = "wiilinkca.pub"

languages = ["ALL", "PTBR", "RU"]


def compile_ssl_cert():
    with open('cert.s', 'w') as f:
        subprocess.run([path_bin2s, "-H", "cert.h", path_cert], stdout=f).check_returncode()

    subprocess.run([path_as, "-o", "cert.o", "cert.s"]).check_returncode()


def build(_language):
    print("Building payload...")

    flags = ["-g", "-Os", "-fPIE", "-std=c++20", '-Wall', '-Werror', "-Wsuggest-override", "-n", "-fno-rtti",
             "-fno-exceptions", "-fno-sized-deallocation", "-ffunction-sections", "-fdata-sections", "-fshort-wchar",
             "-Wl,--gc-sections", "-Wno-address-of-packed-member", f"-DLANGUAGE={_language}"]

    out_path = os.path.join("build", "room")
    binary_path = os.path.join("binary", f"payload.{_language}.bin")
    subprocess.run([path_cc, "-o" + out_path + ".elf", "room.cpp", "cert.o", "-Tpayload.ld",
                    "-I.", "-Wl,--defsym=ORIGIN_ADDRESS=0x80001800"] + flags).check_returncode()
    subprocess.run([path_objcopy, out_path + ".elf", binary_path, "-O", "binary"]).check_returncode()

    with open("binary/00000025.app", "rb") as original:
        data = original.read()
        with open(f"binary/00000025_{_language}.app", "wb") as new:
            with open(binary_path, "rb") as patch:
                new.write(data)
                new.write(patch.read())


if __name__ == "__main__":
    try:
        os.mkdir("build")
    except:
        pass

    try:
        os.mkdir("binary")
    except:
        pass

    compile_ssl_cert()
    for language in languages:
        build(language)
