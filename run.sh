#!/bin/bash
# This script sets up the environment for Sruighlea and runs it.

# Get the directory where the script is located
APP_DIR=$(dirname "$(readlink -f "$0")")

# Set the library path to include the bundled libraries
export LD_LIBRARY_PATH="$APP_DIR/lib":$LD_LIBRARY_PATH

# Set the Qt plugin path to include the bundled plugins
export QT_QPA_PLATFORM_PLUGIN_PATH="$APP_DIR/plugins"

# Run the application
exec "$APP_DIR/Sruighlea" "$@"
