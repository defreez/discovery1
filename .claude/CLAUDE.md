# Discovery One - Project Instructions

## Git Workflow

**Always create a branch based on the GitHub issue before working on it.**

Branch naming convention: `issue-<number>-<short-description>`

Example workflow:
```bash
# Before starting work on issue #3
git checkout main
git pull origin main
git checkout -b issue-3-first-person-camera

# Do work, commit, push
git push -u origin issue-3-first-person-camera

# Create PR referencing the issue
```

## Repository

GitHub: https://github.com/defreez/discovery1

## Tech Stack

- C++20
- raylib 5.5 (fetched via CMake FetchContent)
- Procedural geometry (no external 3D models)
- AI-generated textures

## Build Commands

```bash
cmake -B build
cmake --build build
./build/discovery1
```
