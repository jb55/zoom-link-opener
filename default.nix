{ pkgs ? import <nixpkgs> {} }:
with pkgs;
stdenv.mkDerivation {
  pname = "zoom-link-opener";
  version = "0.1.1";

  makeFlags = "PREFIX=$(out)";

  src = ./.;

  nativeBuildInputs = [ pkg-config gdb valgrind ];
  buildInputs = [ gtk3 libappindicator  ];
}
