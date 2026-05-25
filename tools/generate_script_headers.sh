#!/usr/bin/env bash
set -euo pipefail

src_dir="${1:-scripts}"
out_dir="${2:-generated/lc}"

mkdir -p "$out_dir"

lisp_index="$out_dir/scripts.hpp"
haskell_index="$out_dir/haskell_scripts.hpp"
resources_file="$out_dir/resources.hpp"
tmp_lisp="${lisp_index}.tmp"
tmp_haskell="${haskell_index}.tmp"
tmp_resources="${resources_file}.tmp"

{
    printf '#pragma once\n\n'
} > "$tmp_lisp"

{
    printf '#pragma once\n\n'
} > "$tmp_haskell"

{
    printf '#pragma once\n\n'
    printf '#include "include/lc/core.hpp"\n'
    printf '#include "generated/lc/scripts.hpp"\n'
    printf '#include "generated/lc/haskell_scripts.hpp"\n\n'
    printf 'namespace lc::generated_resources {\n'
    printf 'template<typename Name>\n'
    printf 'struct ResourceByName {\n'
    printf '    static constexpr bool found = false;\n'
    printf '};\n\n'
} > "$tmp_resources"

emit_header() {
    local script="$1"
    local namespace="$2"
    local include_target="$3"
    local ext="$4"

    local base
    base="$(basename "$script" "$ext")"
    local ident="${base//-/_}"
    local delimiter="LC"
    local out_file="$out_dir/${ident}${ext}.hpp"

    {
        printf '#pragma once\n\n'
        printf '#include "include/lc/core.hpp"\n\n'
        printf 'namespace %s {\n' "$namespace"
        printf 'inline constexpr lc::fixed_string %s_source{R"%s(\n' "$ident" "$delimiter"
        cat "$script"
        printf '\n)%s"};\n' "$delimiter"
        printf '} // namespace %s\n' "$namespace"
    } > "$out_file"

    printf '#include "generated/lc/%s%s.hpp"\n' "$ident" "$ext" >> "$include_target"

    local filename
    filename="$(basename "$script")"
    local chars=""
    local i
    for ((i = 0; i < ${#filename}; ++i)); do
        local ch="${filename:i:1}"
        if [[ "$ch" == "'" ]]; then
            chars+="'\\''"
        elif [[ "$ch" == "\\" ]]; then
            chars+="'\\\\'"
        else
            chars+="'$ch'"
        fi
        if (( i + 1 < ${#filename} )); then
            chars+=", "
        fi
    done

    printf 'template<>\n' >> "$tmp_resources"
    printf 'struct ResourceByName<lc::String<%s>> {\n' "$chars" >> "$tmp_resources"
    printf '    static constexpr bool found = true;\n' >> "$tmp_resources"
    printf '    inline static constexpr auto source = %s::%s_source;\n' "$namespace" "$ident" >> "$tmp_resources"
    printf '};\n\n' >> "$tmp_resources"
}

shopt -s nullglob
for script in "$src_dir"/*.lisp; do
    emit_header "$script" "lc::generated_scripts" "$tmp_lisp" ".lisp"
done

for script in "$src_dir"/*.hs; do
    emit_header "$script" "lc::generated_haskell_scripts" "$tmp_haskell" ".hs"
done

printf '} // namespace lc::generated_resources\n' >> "$tmp_resources"

mv "$tmp_lisp" "$lisp_index"
mv "$tmp_haskell" "$haskell_index"
mv "$tmp_resources" "$resources_file"
