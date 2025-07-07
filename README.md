# 🖥️ Yrul OS

**Yrul OS** is a simple, educational operating system built from scratch, focusing on low-level hardware interaction, interrupt handling, and keyboard input management.

**Yrul OS** é um sistema operacional simples e educacional construído do zero, com foco na interação de hardware de baixo nível, tratamento de interrupções e gerenciamento de entrada de teclado.

---

## 🎯 **Project Objectives / Objetivos do Projeto**

### **English**
- Create a minimal OS that boots on x86 architecture
- Implement proper interrupt handling (PIC and IDT configuration)
- Develop a working keyboard driver with PS/2 controller support
- Provide a simple command-line interface
- Demonstrate low-level programming concepts
- Serve as an educational platform for OS development

### **Português**
- Criar um OS mínimo que inicializa na arquitetura x86
- Implementar tratamento adequado de interrupções (configuração PIC e IDT)
- Desenvolver um driver de teclado funcional com suporte ao controlador PS/2
- Fornecer uma interface de linha de comando simples
- Demonstrar conceitos de programação de baixo nível
- Servir como plataforma educacional para desenvolvimento de OS

---

## 🏗️ **Architecture / Arquitetura**

### **Components / Componentes**

#### **Boot System / Sistema de Boot**
- **Multiboot compliant bootloader** / Bootloader compatível com Multiboot
- **Supports both legacy and modern QEMU** / Suporta QEMU legado e moderno
- **32-bit x86 architecture** / Arquitetura x86 de 32 bits

#### **Interrupt Management / Gerenciamento de Interrupções**
- **PIC (Programmable Interrupt Controller) configuration** / Configuração do PIC
- **IDT (Interrupt Descriptor Table) setup** / Configuração da IDT
- **Hardware interrupt handling** / Tratamento de interrupções de hardware

#### **Keyboard System / Sistema de Teclado**
- **PS/2 keyboard controller driver** / Driver do controlador de teclado PS/2
- **Scancode to ASCII conversion** / Conversão de scancode para ASCII
- **Interrupt-based input with polling fallback** / Entrada baseada em interrupções com fallback para polling
- **Command line interface** / Interface de linha de comando

#### **Display System / Sistema de Exibição**
- **VGA text mode (80x25)** / Modo texto VGA (80x25)
- **Direct video memory manipulation** / Manipulação direta da memória de vídeo
- **Color-coded output** / Saída com códigos de cor

---

## 🔧 **Building and Running / Compilação e Execução**

### **Prerequisites / Pré-requisitos**

#### **Required Tools / Ferramentas Necessárias**
```bash
# Ubuntu/Debian
sudo apt-get install gcc-multilib nasm qemu-system-x86

# Arch Linux
sudo pacman -S gcc-multilib nasm qemu-system-i386

# Fedora/RHEL
sudo dnf install gcc gcc-multilib nasm qemu-system-x86
```

### **Building / Compilação**

```bash
# Clone the repository / Clone o repositório
git clone [repository-url]
cd Yrul3-0/yrul-os

# Build the OS / Compile o OS
make

# Clean build files / Limpe os arquivos de build
make clean
```

### **Running / Execução**

#### **Standard Execution / Execução Padrão**
```bash
make run
```

#### **Debug Mode / Modo Debug**
```bash
make run-debug
# Use Ctrl+Alt+2 for QEMU monitor
# Use Ctrl+Alt+1 to return to OS display
```

#### **Create Bootable ISO / Criar ISO Bootável**
```bash
make iso
# Creates yrul.iso for CD/DVD boot
```

### **Testing on Real Hardware / Teste em Hardware Real**

#### **USB Installation / Instalação em USB**
```bash
# WARNING: This will erase the USB device
# ATENÇÃO: Isso apagará o dispositivo USB
make install-usb USB_DEVICE=/dev/sdX
```

---

## 🎮 **Usage / Uso**

### **User Interface / Interface do Usuário**

#### **Boot Process / Processo de Boot**
1. **System initializes** / Sistema inicializa
2. **Hardware components configured** / Componentes de hardware configurados
3. **Keyboard driver loaded** / Driver de teclado carregado
4. **Command prompt appears** / Prompt de comando aparece

#### **Keyboard Input / Entrada de Teclado**
- **Type characters** on line 22 / Digite caracteres na linha 22
- **Press Enter** to execute commands / Pressione Enter para executar comandos
- **Use Backspace** to correct mistakes / Use Backspace para corrigir erros
- **Visual feedback** with cursor display / Feedback visual com exibição do cursor

#### **System Indicators / Indicadores do Sistema**
- **Top-right corner**: Interrupt count and scancode display / Canto superior direito: Contagem de interrupções e exibição de scancode
- **Bottom area**: System status and mode indicators / Área inferior: Status do sistema e indicadores de modo
- **Line 23**: Processed commands display / Linha 23: Exibição de comandos processados

---

## 🛠️ **Technical Details / Detalhes Técnicos**

### **File Structure / Estrutura de Arquivos**

```
yrul-os/
├── boot/
│   ├── boot.s          # Multiboot bootloader
│   ├── grub.cfg        # GRUB configuration
│   └── linker.ld       # Linker script
├── src/
│   ├── kernel.c        # Main kernel
│   ├── interrupts.c    # Interrupt management
│   ├── keyboard.c      # Keyboard driver
│   ├── io.c           # I/O operations
│   ├── pic_acknowledge.s # PIC interrupt acknowledgment
│   └── *.h            # Header files
└── Makefile           # Build system
```

### **Memory Layout / Layout de Memória**
- **Kernel loaded at**: 1MB (0x100000) / Kernel carregado em: 1MB
- **Video memory**: 0xB8000 (VGA text mode) / Memória de vídeo: 0xB8000
- **Stack**: High memory, grows downward / Stack: Memória alta, cresce para baixo

### **Interrupt Configuration / Configuração de Interrupções**
- **PIC remapped**: Master (0x20-0x27), Slave (0x28-0x2F) / PIC remapeado
- **Keyboard IRQ**: IRQ1 (INT 0x21) / IRQ do teclado: IRQ1
- **IDT size**: 256 entries / Tamanho da IDT: 256 entradas

---

## 🚀 **Features / Funcionalidades**

### **Current Features / Funcionalidades Atuais**
- ✅ **Multiboot-compliant bootloader** / Bootloader compatível com Multiboot
- ✅ **Hardware interrupt handling** / Tratamento de interrupções de hardware
- ✅ **PS/2 keyboard driver** / Driver de teclado PS/2
- ✅ **VGA text mode display** / Exibição em modo texto VGA
- ✅ **Command line interface** / Interface de linha de comando
- ✅ **Automatic interrupt/polling fallback** / Fallback automático interrupção/polling
- ✅ **Real-time system monitoring** / Monitoramento do sistema em tempo real

### **Planned Features / Funcionalidades Planejadas**
- 🔄 **Basic command interpreter** / Interpretador básico de comandos
- 🔄 **Memory management** / Gerenciamento de memória
- 🔄 **Basic file system** / Sistema de arquivos básico
- 🔄 **Process management** / Gerenciamento de processos
- 🔄 **Timer interrupts** / Interrupções de timer

---

## 🐛 **Troubleshooting / Solução de Problemas**

### **Common Issues / Problemas Comuns**

#### **Keyboard Not Working / Teclado Não Funciona**
**English**: The system automatically detects if interrupts are not working and switches to polling mode. Look for "POLL" indicator in the bottom-right corner.

**Português**: O sistema detecta automaticamente se as interrupções não estão funcionando e muda para modo polling. Procure pelo indicador "POLL" no canto inferior direito.

#### **System Hangs / Sistema Trava**
**English**: Ensure QEMU version compatibility. Try different QEMU parameters or test on real hardware.

**Português**: Verifique a compatibilidade da versão do QEMU. Tente parâmetros diferentes do QEMU ou teste em hardware real.

#### **Build Errors / Erros de Compilação**
**English**: Ensure all required tools are installed and you have proper 32-bit compilation support.

**Português**: Verifique se todas as ferramentas necessárias estão instaladas e se você tem suporte adequado para compilação de 32 bits.

---

## 🎓 **Educational Value / Valor Educacional**

### **Learning Outcomes / Resultados de Aprendizagem**

#### **For Students / Para Estudantes**
- **Low-level programming concepts** / Conceitos de programação de baixo nível
- **Hardware-software interaction** / Interação hardware-software
- **Interrupt handling mechanisms** / Mecanismos de tratamento de interrupções
- **Memory management basics** / Fundamentos de gerenciamento de memória
- **Assembly language integration** / Integração com linguagem assembly

#### **For Developers / Para Desenvolvedores**
- **OS development fundamentals** / Fundamentos de desenvolvimento de OS
- **Cross-compilation techniques** / Técnicas de compilação cruzada
- **Hardware emulation with QEMU** / Emulação de hardware com QEMU
- **Build system design** / Design de sistema de build
- **Real-time system programming** / Programação de sistemas em tempo real

---

## 🤝 **Contributing / Contribuindo**

### **Development Guidelines / Diretrizes de Desenvolvimento**

#### **Code Style / Estilo de Código**
- **Clean, readable code** / Código limpo e legível
- **Comprehensive comments** / Comentários abrangentes
- **Consistent naming conventions** / Convenções de nomenclatura consistentes
- **Modular design** / Design modular

#### **Testing / Testes**
- **Test on multiple QEMU versions** / Teste em múltiplas versões do QEMU
- **Verify on real hardware when possible** / Verifique em hardware real quando possível
- **Document any compatibility issues** / Documente problemas de compatibilidade

---

## 📜 **License / Licença**

This project is released under the MIT License. See LICENSE file for details.

Este projeto é liberado sob a Licença MIT. Veja o arquivo LICENSE para detalhes.

---

## 🙏 **Acknowledgments / Agradecimentos**

### **References / Referências**
- **OSDev Wiki**: Comprehensive OS development resource
- **Intel Architecture Manuals**: Hardware specification reference
- **QEMU Documentation**: Emulation platform guidance
- **GCC Cross-Compilation**: Toolchain setup information

### **Inspiration / Inspiração**
This project was inspired by educational OS projects and the need for a simple, understandable example of low-level system programming.

Este projeto foi inspirado por projetos educacionais de OS e a necessidade de um exemplo simples e compreensível de programação de sistemas de baixo nível.

---

## 📞 **Contact / Contato**

For questions, suggestions, or contributions, please open an issue or submit a pull request.

Para dúvidas, sugestões ou contribuições, por favor abra uma issue ou envie um pull request.

**Project Status**: Active Development / Desenvolvimento Ativo  
**Last Updated**: July 2025 / Última Atualização: Julho 2025
