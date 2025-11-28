#!/usr/bin/env bash
set -e

# Hardcoded configuration
LOCAL_DIR="./"
REMOTE="mcng@linux.student.cs.uwaterloo.ca"
REMOTE_PATH="/u2/mcng/cs246/1259/project/remote_build"
ARTIFACT_PATTERN="biquadris"

REMOTE_DIR="$REMOTE_PATH/$(basename "$LOCAL_DIR")"

echo "Syncing directory to remote..."
rsync -avz --delete "$LOCAL_DIR" "$REMOTE":"$REMOTE_PATH"

echo "Running make on remote..."
ssh "$REMOTE" "cd '$REMOTE_DIR' && make rebuild"

echo "Downloading build artifacts matching pattern: $ARTIFACT_PATTERN"
rsync -avz "$REMOTE":"$REMOTE_DIR/$ARTIFACT_PATTERN" ./ 

echo "Done."