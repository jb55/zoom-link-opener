{ pkgs ? import <nixpkgs> {} }:
with pkgs;
stdenv.mkDerivation {
  name = "zoom-link-opener";
  nativeBuildInputs = [ pkgconfig gdb ];
  buildInputs = [ gtk3 libappindicator  ];
}
