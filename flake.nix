{
  description = "A compositor framework for Flutter";

  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs/release-24.11";
    flutter-327-nixpkgs.url = "github:NixOS/nixpkgs?ref=pull/365302/head";
    systems.url = "github:nix-systems/default-linux";
    flake-utils.url = "github:numtide/flake-utils";
    nixos-apple-silicon = {
      url = "github:tpwrules/nixos-apple-silicon/release-2024-12-25";
      inputs.nixpkgs.follows = "nixpkgs";
    };
    shoyu = {
      url = "github:MidstallSoftware/shoyu";
      inputs = {
        nixpkgs.follows = "nixpkgs";
        systems.follows = "systems";
        flake-utils.follows = "flake-utils";
        nixos-apple-silicon.follows = "nixos-apple-silicon";
      };
    };
  };

  outputs =
    {
      self,
      nixpkgs,
      flutter-327-nixpkgs,
      systems,
      flake-utils,
      nixos-apple-silicon,
      shoyu,
      ...
    }:
    let
      inherit (nixpkgs) lib;

      mkDevShell =
        pkgs:
        pkgs.mkShell {
          pname = "miso";
          version = self.shortRev or "dirty";

          packages = with pkgs; [
            pkgs.shoyu
            flutterPackages-source.v3_27
            pkg-config
            gtk3
            yq
            cmake
            ninja
            llvmPackages.clang
          ];
        };
    in
    flake-utils.lib.eachSystem (import systems) (
      system:
      let
        pkgs = nixpkgs.legacyPackages.${system}.appendOverlays [
          (
            pkgs: prev: with pkgs; {
              pkgsAsahi = (
                if stdenv.hostPlatform.isAarch64 then
                  pkgs.appendOverlays [
                    nixos-apple-silicon.overlays.default
                    (pkgsAsahi: prev: {
                      mesa = pkgsAsahi.mesa-asahi-edge.overrideAttrs (f: p: {
                        meta.platforms = prev.mesa.meta.platforms;
                      });
                    })
                  ]
                else
                  null
              );

              flutterPackages-source = pkgs.callPackage "${flutter-327-nixpkgs}/pkgs/development/compilers/flutter" {
                useNixpkgsEngine = true;
              };
            }
          )
          shoyu.overlays.default
        ];
      in
      {
        legacyPackages = pkgs;

        devShells =
          {
            default = mkDevShell pkgs;
          }
          // lib.optionalAttrs (pkgs.pkgsAsahi != null) {
            asahi = mkDevShell pkgs.pkgsAsahi;
          };
      }
    );
}
