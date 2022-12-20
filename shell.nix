with import <nixpkgs> { };
mkShellNoCC {
  buildInputs = [
    gnat11 # gcc with ada
    #gnatboot # gnat1
    ncurses # make menuconfig
    m4 flex bison # Generate flashmap descriptor parser
    #clang
    #iasl
    zlib
    acpica-tools # iasl
    pkgconfig
    qemu # test the image
    libuuid.dev
    nasm
    imagemagick
    python3
 ];
  shellHook = ''
    # TODO remove?
    NIX_LDFLAGS="$NIX_LDFLAGS -lncurses"
  '';
}

