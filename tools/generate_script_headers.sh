#!/usr/bin/env bash
set -euo pipefail

src_dir="${1:-scripts}"
out_dir="${2:-generated/lc}"

mkdir -p "$out_dir"

index_file="$out_dir/scripts.hpp"
tmp_index="${index_file}.tmp"

{
    printf '#pragma once\n\n'
} > "$tmp_index"

shopt -s nullglob
for script in "$src_dir"/*.lisp; do
    base="$(basename "$script" .lisp)"
    ident="${base//-/_}"
    upper_ident="$(printf '%s' "$ident" | tr '[:lower:]' '[:upper:]')"
    delimiter="LCS_${upper_ident}"
    out_file="$out_dir/${ident}.hpp"

    {
        printf '#pragma once\n\n'
        printf '#include "include/lc/core.hpp"\n\n'
        printf 'namespace lc::generated_scripts {\n'
        printf 'inline constexpr lc::fixed_string %s_source{R"%s(\n' "$ident" "$delimiter"
        cat "$script"
        printf '\n)%s"};\n' "$delimiter"
        printf '} // namespace lc::generated_scripts\n'
    } > "$out_file"

    printf '#include "generated/lc/%s.hpp"\n' "$ident" >> "$tmp_index"
done

mv "$tmp_index" "$index_file"
