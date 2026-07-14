#!/usr/bin/env python3
"""Create a sorted ZIP with fixed timestamps while preserving Unix modes."""

from __future__ import annotations

import argparse
import os
import stat
import zipfile
from pathlib import Path


FIXED_TIME = (1980, 1, 1, 0, 0, 0)


def add_path(archive: zipfile.ZipFile, source: Path, relative: Path) -> None:
    info = zipfile.ZipInfo(relative.as_posix(), FIXED_TIME)
    mode = source.lstat().st_mode
    info.create_system = 3
    info.external_attr = mode << 16

    if source.is_symlink():
        info.external_attr = (stat.S_IFLNK | 0o777) << 16
        archive.writestr(info, os.readlink(source).encode("utf-8"))
        return

    info.compress_type = zipfile.ZIP_DEFLATED
    with source.open("rb") as input_file, archive.open(info, "w") as output_file:
        while chunk := input_file.read(1024 * 1024):
            output_file.write(chunk)


def main() -> int:
    parser = argparse.ArgumentParser()
    parser.add_argument("source", type=Path)
    parser.add_argument("destination", type=Path)
    args = parser.parse_args()

    source = args.source.resolve(strict=True)
    destination = args.destination.resolve()
    destination.parent.mkdir(parents=True, exist_ok=True)
    destination.unlink(missing_ok=True)

    files = sorted(
        (path for path in source.rglob("*") if path.is_file() or path.is_symlink()),
        key=lambda path: path.relative_to(source).as_posix(),
    )
    if not files:
        raise RuntimeError(f"Refusing to create an empty ZIP from {source}")

    with zipfile.ZipFile(destination, "w", allowZip64=True) as archive:
        for path in files:
            if path.name == ".DS_Store" or path.name.startswith("._"):
                continue
            add_path(archive, path, path.relative_to(source))

    return 0


if __name__ == "__main__":
    raise SystemExit(main())
