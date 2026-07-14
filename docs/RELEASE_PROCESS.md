# Release process

1. Update the single authoritative `VERSION.txt` value.
2. Update `CHANGELOG.md` and any version-dependent installer licence text.
3. Run local Release tests and extended stress tests.
4. Commit the verified source.
5. Push the branch.
6. Confirm Windows CI succeeds.
7. Create `vX.Y.Z` at that exact commit.
8. Push the tag.
9. Wait for Build Windows Release.
10. Review tests and diagnostics.
11. Review validator status honestly.
12. Download, checksum, inspect, and install the packages.
13. Dispatch Publish GitHub Release with the tag and exact successful run ID.
14. Download the published installer and verify its checksum.
15. Perform final install, host scan, audio, state, preset, and uninstall smoke tests.

This is a **Repeatable clean CI build** process, not a claim of bit-for-bit reproducibility.

