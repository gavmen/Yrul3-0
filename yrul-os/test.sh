#!/bin/bash

# Yrul OS Test Script - ENHANCED VERSION
# Script de Teste do Yrul OS - VERSÃO MELHORADA

echo "🖥️  YRUL OS - ENHANCED TEST EXECUTION"
echo "🖥️  YRUL OS - EXECUÇÃO DE TESTE MELHORADA"
echo "========================================="

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
echo "🎯 ENHANCED SYSTEM FEATURES / FUNCIONALIDADES MELHORADAS:"
echo "   • ✅ Fixed keyboard mapping (q→q, e→e, correct characters)"
echo "   • ✅ Anti-duplication system (no more double characters)"
echo "   • ✅ Persistent command display (stays visible longer)"
echo "   • ✅ Enhanced command system with 6 commands"
echo "   • ✅ Improved debug indicators"
echo "   • ✅ Better error handling and feedback"
echo ""
echo "📋 AVAILABLE COMMANDS / COMANDOS DISPONÍVEIS:"
echo "   • help  - Show all available commands"
echo "   • clear - Clear command display area"
echo "   • info  - Detailed system information"
echo "   • test  - Display colorful test pattern"
echo "   • ver   - Show OS version information"
echo "   • [any] - Unknown command handling"
echo ""
echo "🎮 CONTROLS / CONTROLES:"
echo "   • Type to input text / Digite para inserir texto"
echo "   • Enter to execute command / Enter para executar comando"
echo "   • Backspace to correct / Backspace para corrigir"
echo "   • Ctrl+C to exit QEMU / Ctrl+C para sair do QEMU"
echo ""
echo "🔍 DEBUG INDICATORS / INDICADORES DE DEBUG:"
echo "   • Top-right corner: Interrupt count, scancode (hex), ASCII char"
echo "   • Bottom-right: System mode (IRQ/POLL) and execution status"
echo "   • Line 23: Processed commands (persistent display)"
echo "   • Real-time feedback for all user interactions"
echo ""
echo "🐛 FIXES APPLIED / CORREÇÕES APLICADAS:"
echo "   • ✅ Scancode to ASCII mapping corrected"
echo "   • ✅ Anti-duplication system improved"
echo "   • ✅ Command persistence timer implemented"
echo "   • ✅ Enhanced visual feedback"
echo "   • ✅ Robust error handling"
echo ""

read -p "🚀 Press Enter to start Enhanced Yrul OS / Pressione Enter para iniciar Yrul OS Melhorado..."

echo "🎮 Starting Enhanced Yrul OS / Iniciando Yrul OS Melhorado..."
echo "   Test the keyboard fixes by typing: q, e, a, help, info, test"
echo "   Using stable mode to prevent QEMU pause issues"
echo ""
make run-stable

echo ""
echo "🏁 Yrul OS test completed / Teste do Yrul OS finalizado!"
