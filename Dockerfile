                   # Use Ubuntu as base image
FROM ubuntu:22.04

# Avoid prompts from apt
ENV DEBIAN_FRONTEND=noninteractive

# Install dependencies including OpenCV
RUN apt-get update && apt-get install -y \
    build-essential \
    cmake \
    libopencv-dev \
    && rm -rf /var/lib/apt/lists/*

# Set working directory
WORKDIR /app

# Copy project files
COPY . .

# Create build directory and build the project
RUN mkdir -p build && \
    cd build && \
    cmake .. && \
    cmake --build .

# Set the entrypoint to run the executable
CMD ["./build/main"]
