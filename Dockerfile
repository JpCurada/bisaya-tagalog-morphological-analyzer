FROM python:3.11-slim

# Install build dependencies for C compilation
RUN apt-get update && apt-get install -y \
    gcc \
    build-essential \
    && rm -rf /var/lib/apt/lists/*

# Set working directory
WORKDIR /app

# Copy requirements first for better caching
COPY requirements.txt .
RUN pip install --no-cache-dir -r requirements.txt

# Copy source code and data
COPY src/ ./src/
COPY data/ ./data/
COPY web/ ./web/

# Compile C code
WORKDIR /app/src/c
RUN gcc -shared -fPIC -o libanalyzer.so \
    analyzer.c \
    morphology.c \
    utils.c \
    lexicon.c

# Return to app directory
WORKDIR /app

# Expose port
EXPOSE 8000

# Set environment variables
ENV PYTHONUNBUFFERED=1
ENV FLASK_ENV=production

# Run the server
CMD ["python", "src/python/server.py"]
