{
  inputs = {
    nixpkgs.url = github:NixOS/nixpkgs/nixos-21.11;
  };
  outputs = { self, nixpkgs }:
    let
      system = "x86_64-linux";
      pkgs = import nixpkgs { inherit system; };
      getPackage = pkgs: pkgs.stdenv.mkDerivation rec {
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
      packages.${system}.default = getPackage pkgs;
      overlays.default = final: prev: {
        getworkspacename = getPackage final;
      };
    };
}

