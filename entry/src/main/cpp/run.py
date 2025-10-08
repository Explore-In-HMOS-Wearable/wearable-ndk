#!/usr/bin/env python3
# -*- coding: utf-8 -*-

import os
import glob
from pathlib import Path

def collect_scripts(root_dir="."):
    """
    Belirtilen dizin ve alt dizinlerdeki tüm script dosyalarını toplar
    """
    # Script uzantıları - ihtiyacınıza göre ekleyebilir/çıkarabilirsiniz
    script_extensions = [
        '*.py',      # Python
        '*.js',      # JavaScript
        '*.ts',      # TypeScript
        '*.sh',      # Shell script
        '*.bat',     # Batch
        '*.ps1',     # PowerShell
        '*.php',     # PHP
        '*.rb',      # Ruby
        '*.pl',      # Perl
        '*.r',       # R
        '*.sql',     # SQL
        '*.cpp',     # C++
        '*.c',       # C
        '*.java',    # Java
        '*.cs',      # C#
        '*.go',      # Go
        '*.rs',      # Rust
    ]
    
    script_files = []
    
    # Her uzantı için dosyaları ara
    for extension in script_extensions:
        # Alt dizinler dahil (**/ ile recursive arama)
        pattern = os.path.join(root_dir, "**", extension)
        files = glob.glob(pattern, recursive=True)
        script_files.extend(files)
    
    # Dosyaları alfabetik olarak sırala
    script_files.sort()
    
    return script_files

def read_file_safely(file_path):
    """
    Dosyayı güvenli bir şekilde okur, encoding hatalarını yönetir
    """
    encodings = ['utf-8', 'latin-1', 'cp1252', 'iso-8859-1']
    
    for encoding in encodings:
        try:
            with open(file_path, 'r', encoding=encoding) as f:
                return f.read()
        except UnicodeDecodeError:
            continue
        except Exception as e:
            return f"# HATA: Dosya okunamadı - {str(e)}"
    
    return "# HATA: Hiçbir encoding ile dosya okunamadı"

def create_prompt_file(output_file="merged_scripts_prompt.txt"):
    """
    Tüm scriptleri tek bir prompt dosyasında birleştirir
    """
    current_dir = os.getcwd()
    script_files = collect_scripts(current_dir)
    
    if not script_files:
        print("Hiç script dosyası bulunamadı!")
        return
    
    print(f"Toplam {len(script_files)} script dosyası bulundu.")
    
    with open(output_file, 'w', encoding='utf-8') as output:
        # Başlık ve genel bilgi
        output.write("=" * 80 + "\n")
        output.write("TÜM SCRIPT DOSYALARI - PROMPT\n")
        output.write("=" * 80 + "\n")
        output.write(f"Ana Dizin: {current_dir}\n")
        output.write(f"Toplam Dosya Sayısı: {len(script_files)}\n")
        output.write(f"Oluşturulma Tarihi: {Path().cwd()}\n")
        output.write("=" * 80 + "\n\n")
        
        # İngilizce talimat - prompt'un bu kısmı almaması için
        output.write("IMPORTANT INSTRUCTION FOR AI:\n")
        output.write("Please ignore the header information above (file list, directory info, etc.) ")
        output.write("when processing this prompt. Focus only on the actual code content below.\n")
        output.write("The header is just for organization purposes.\n")
        output.write("=" * 80 + "\n\n")
        
        # Dosya listesi
        output.write("DOSYA LİSTESİ:\n")
        output.write("-" * 40 + "\n")
        for i, file_path in enumerate(script_files, 1):
            # Relative path göster
            rel_path = os.path.relpath(file_path, current_dir)
            output.write(f"{i:3d}. {rel_path}\n")
        output.write("-" * 40 + "\n\n")
        
        # Her dosyanın içeriğini ekle
        output.write("ACTUAL CODE CONTENT STARTS HERE:\n")
        output.write("=" * 80 + "\n\n")
        
        for file_path in script_files:
            rel_path = os.path.relpath(file_path, current_dir)
            file_size = os.path.getsize(file_path)
            
            output.write("=" * 80 + "\n")
            output.write(f"DOSYA: {rel_path}\n")
            output.write(f"Boyut: {file_size} bytes\n")
            output.write("=" * 80 + "\n")
            
            # Dosya içeriğini oku
            content = read_file_safely(file_path)
            
            if content.strip():
                output.write(content)
            else:
                output.write("# Dosya boş veya okunamadı")
            
            output.write("\n\n")
    
    print(f"✅ Tüm scriptler '{output_file}' dosyasına birleştirildi!")
    print(f"📁 Dosya konumu: {os.path.abspath(output_file)}")

def main():
    """
    Ana fonksiyon
    """
    print("🔍 Script dosyaları aranıyor...")
    
    # Çıktı dosyası adını özelleştirebilirsiniz
    output_filename = "merged_scripts_prompt.txt"
    
    # İsteğe bağlı: Kullanıcıdan dosya adı al
    custom_name = input(f"Çıktı dosyası adı (Enter = '{output_filename}'): ").strip()
    if custom_name:
        output_filename = custom_name
        if not output_filename.endswith('.txt'):
            output_filename += '.txt'
    
    create_prompt_file(output_filename)

if __name__ == "__main__":
    main()