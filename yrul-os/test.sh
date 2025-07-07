#!/bin/bash

# Yrul OS Test Script
# Script de Teste do Yrul OS

echo "🖥️  YRUL OS - TEST EXECUTION"
echo "🖥️  YRUL OS - EXECUÇÃO DE TESTE"
echo "================================="

# Check if we're in the right directory
if [ ! -f "Makefile" ]; then
    echo "❌ Error: Not in yrul-os directory / Erro: Não está no diretório yrul-os"
    echo "Please run from yrul-os/ directory / Execute do diretório yrul-os/"
    exit 1
fi

# Build the OS
echo "🔧 Building Yrul OS / Compilando Yrul OS..."
if make clean > /dev/null 2>&1 && make > /dev/null 2>&1; then
    echo "✅ Build successful / Compilação bem-sucedida!"
else
    echo "❌ Build failed / Falha na compilação!"
    echo "Run 'make' manually to see errors / Execute 'make' manualmente para ver erros"
    exit 1
fi

echo ""
echo "🎯 SYSTEM FEATURES / FUNCIONALIDADES DO SISTEMA:"
echo "   • Keyboard input on line 22 / Entrada de teclado na linha 22"
echo "   • Command processing / Processamento de comandos"
echo "   • Interrupt or polling mode / Modo interrupção ou polling"
echo "   • Real-time status indicators / Indicadores de status em tempo real"
echo ""
echo "📋 CONTROLS / CONTROLES:"
echo "   • Type to input text / Digite para inserir texto"
echo "   • Enter to execute command / Enter para executar comando"
echo "   • Backspace to correct / Backspace para corrigir"
echo "   • Ctrl+C to exit QEMU / Ctrl+C para sair do QEMU"
echo ""
echo "🔍 INDICATORS / INDICADORES:"
echo "   • Top-right: Interrupt count & scancode / Superior direito: Contador e scancode"
echo "   • Bottom-right: System mode (IRQ/POLL) / Inferior direito: Modo do sistema"
echo "   • Line 23: Processed commands / Linha 23: Comandos processados"
echo ""

read -p "🚀 Press Enter to start Yrul OS / Pressione Enter para iniciar o Yrul OS..."

echo "🎮 Starting Yrul OS / Iniciando Yrul OS..."
qemu-system-i386 -kernel yrul.bin -m 32M -no-reboot -no-shutdown

echo ""
echo "🏁 Yrul OS test completed / Teste do Yrul OS finalizado!"
