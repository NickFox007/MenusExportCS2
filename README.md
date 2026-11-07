# MenusExportCS2

Metamod:Source helper plugin that exports the legacy C ABI used by the CSSharp `MenuManager` plugin and forwards calls to Pisex's `IMenusApi`.

## Target and dependencies

- Linux x86_64 / Counter-Strike 2 (Source 2)
- Metamod:Source 2.0 source tree
- HL2SDK CS2 branch and its `hl2sdk-manifests`
- AMBuild 2.2 or newer
- GCC or Clang toolchain

The bundled `include/menus.h` follows the current [Pisex/cs2-menus API](https://github.com/Pisex/cs2-menus/blob/main/include/menus.h). Update it from upstream when that API changes; the plugin must be built against the same interface revision as the installed Menus plugin.

## Build on Linux

Clone the current dependencies (or use existing checkouts):

```sh
git clone --recursive --branch master https://github.com/alliedmodders/metamod-source.git
# HL2SDK manifests are included in current Metamod:Source checkouts.
git clone --recursive --branch cs2 https://github.com/alliedmodders/hl2sdk.git hl2sdk-cs2
git clone https://github.com/alliedmodders/ambuild.git
python3 -m pip install ./ambuild
```

Configure and build from the plugin repository root:

```sh
python3 configure.py \
  --mms_path=/path/to/metamod-source \
  --hl2sdk-manifests=/path/to/metamod-source/hl2sdk-manifests \
  --hl2sdk-root=/path/to \
  --sdks=cs2 \
  --targets=x86_64 \
  --enable-optimize
ambuild
```

The build output/package is under `build/package`. The plugin VDF and Linux x86_64 binary are packaged under `addons/metamod` and `addons/menus_export/bin` respectively.

If the SDK manifests are not in the Metamod tree, provide their actual path with `--hl2sdk-manifests`. `--hl2sdk-root` should contain the `hl2sdk-cs2` directory.
