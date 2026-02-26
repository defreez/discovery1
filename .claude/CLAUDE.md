# Discovery One - Project Instructions

## Git Workflow

**Always create a branch based on the GitHub issue before working on it.**

Branch naming convention: `issue-<number>-<short-description>`

**Commit often, squash merge at the end.**
- Make many small commits as you work
- Push frequently
- When PR is ready, squash merge to keep main history clean

Example workflow:
```bash
# Before starting work on issue #3
git checkout main
git pull origin main
git checkout -b issue-3-first-person-camera

# Do work, commit frequently
git add <files>
git commit -m "Add camera struct"
git commit -m "Implement mouse look"
git commit -m "Add WASD movement"
git push -u origin issue-3-first-person-camera

# Create PR, then squash merge
gh pr create --title "First-person camera" --body "Closes #3"
gh pr merge --squash
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
