{
  inputs = {
    nixpkgs.url = "github:nixos/nixpkgs/nixos-unstable";
  };

  outputs =
    { self, nixpkgs, ... }:
    let
      system = "x86_64-linux";
    in
    {
      devShells.${system}.default =
        let
          pkgs = import nixpkgs { inherit system; };
        in
        pkgs.mkShell.override { stdenv = pkgs.gcc14Stdenv; } {

          nativeBuildInputs = with pkgs; [
            cmake
            ninja
            pkg-config
            vulkan-tools
            glslang
            renderdoc
          ];

          buildInputs = with pkgs; [
            vulkan-headers
            vulkan-loader
            vulkan-validation-layers
            glfw
            glm
          ];

          shellHook = ''
            export VULKAN_SDK="${pkgs.vulkan-headers}"
            export VK_LAYER_PATH="${pkgs.vulkan-validation-layers}/share/vulkan/explicit_layer.d"
          '';
        };
    };
}
