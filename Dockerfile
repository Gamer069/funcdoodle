# Use an official Linux-based image
FROM ubuntu:latest

# Set environment variables to prevent interactive prompts during package installation
ENV DEBIAN_FRONTEND=noninteractive

# Install dependencies (if needed)
RUN echo -e "deb http://archive.ubuntu.com/ubuntu/ focal universe\n\
deb-src http://archive.ubuntu.com/ubuntu/ focal universe" | tee -a /etc/apt/sources.list.d/ubuntu.sources && apt-get update && apt-get install --fix-missing -y \
    bash \
    clang \
    curl \
    build-essential \
    cmake \
    libwayland-dev \
    libwayland-egl-backend-dev \
    libwayland-client0 \
    libxkbcommon-dev \
    libgtk-3-dev \
    libgdk-pixbuf2.0-dev \
    libinput-dev \
    libudev-dev \
    wayland-protocols \
	portaudio19-dev \
	libportaudio2 \
    && rm -rf /var/lib/apt/lists/*

# Create a directory for the build output
RUN mkdir -p /build_linux

RUN cp /usr/lib/aarch64-linux-gnu/libportaudio.so.2 /usr/lib/libportaudio.so

# Copy the build script into the container
COPY . /funcdoodle

WORKDIR /funcdoodle

# Give execute permissions to the build script
RUN chmod +x /funcdoodle/build.sh

# Run the build script, move the output to /build_linux, and print a completion message
CMD ["sh", "-c", "cd /funcdoodle; ./build.sh && mv build/* /build_linux/ && echo 'Build completed!'"]
