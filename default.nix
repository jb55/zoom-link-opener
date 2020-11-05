{ pkgs ? import <nixpkgs> {} }:
with pkgs;
stdenv.mkDerivation {
  pname = "zoom-link-opener";
  version = "0.1";

  makeFlags = "PREFIX=$(out)";

  src = ./.;

  nativeBuildInputs = [ pkgconfig gdb valgrind ];
  buildInputs = [ gtk3 libappindicator  ];
}
