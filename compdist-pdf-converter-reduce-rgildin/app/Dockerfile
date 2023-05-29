# Base image
FROM python:3.7

# Update the package list and install dependencies
RUN apt-get update && apt-get install -y \
    poppler-utils \
    ghostscript

# Set the working directory
WORKDIR /app

# Copy the application code
COPY . .

# Install dependencies
RUN pip install -r requirements.txt

# Expose port
EXPOSE 5000

# Set the entry point
CMD ["gunicorn", "app:app", "-b", "0.0.0.0:5000"]
