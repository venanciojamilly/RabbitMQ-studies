#!/bin/bash

echo "Atualizando pacotes..."
sudo apt update && sudo apt upgrade -y

echo "Instalando CMake..."
sudo apt install -y cmake

echo "Instalando compilador C (GCC e Clang)..."
sudo apt install -y gcc g++ clang

echo "Instalando OpenSSL..."
sudo apt install -y libssl-dev

echo "Instalando POpt (opcional)..."
sudo apt install -y libpopt-dev

echo "Instalando XmlTo (opcional)..."
sudo apt install -y xmlto

echo "Instalando Doxygen (opcional)..."
sudo apt install -y doxygen

echo "Instalando RabbitMQ C Client (librabbitmq)..."
sudo apt install -y librabbitmq-dev

echo "Todas as dependências foram instaladas!"
