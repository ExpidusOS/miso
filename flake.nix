{
  description = "A compositor framework for Flutter";

  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs";
    systems.url = "github:nix-systems/default-linux";
    flake-utils.url = "github:numtide/flake-utils";
    nixos-apple-silicon = {
      url = "github:tpwrules/nixos-apple-silicon/release-2024-11-12";
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
            flutter327
            pkg-config
            gtk3
            yq
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
                      mesa = pkgsAsahi.mesa-asahi-edge;
                    })
                  ]
                else
                  null
              );
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
