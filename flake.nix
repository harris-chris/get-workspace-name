{
  inputs = {
    nixpkgs.url = github:NixOS/nixpkgs/nixos-21.11;
    flake-utils.url = github:numtide/flake-utils;
  };
  outputs = { self, nixpkgs, flake-utils }:
    flake-utils.lib.eachDefaultSystem (system:
      let
        pkgs = nixpkgs.legacyPackages.${system};
        getworkspacename = pkgs.stdenv.mkDerivation rec {
          name = "getworkspacename";
          src = ./getWorkspaceName.cpp;
          buildInputs = [ pkgs.coreutils ];
          dontUnpack = true;
          buildPhase = ''
            g++ -o getworkspacename $src
          '';
          installPhase = ''
            mkdir -p $out/bin
            cp getworkspacename $out/bin
            ln -s ${pkgs.wmctrl.out}/bin/wmctrl $out/bin/wmctrl
            chmod +x $out/bin/getworkspacename
          '';
          gcc = pkgs.gcc;
        };
      in {
        defaultPackage = getworkspacename;
        devShell = pkgs.mkShell {
          buildInputs = [ getworkspacename ];
        };
      });
}

