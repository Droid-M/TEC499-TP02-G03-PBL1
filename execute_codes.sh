#!/bin/bash

while true; do
  # Exibe as opções para o usuário
  echo "Escolha uma opção:"
  echo "1: Executar o Menu principal"
  echo "2: Executar o Testador simples de conexão UART"
  echo "3: Sair"
  
  # Lê a escolha do usuário
  read -p "Sua escolha: " option
  
  # Verifica a opção escolhida e executa o correspondente
  case $option in
    1)
      clear
      ./PC/Executables/menu
      break  # Sai do loop se a opção for válida
      ;;
    2)
      clear    
      ./PC/Executables/simple_communication
      break  # Sai do loop se a opção for válida
      ;;
    3)
      echo "Saindo..."
      break
      ;;
    *)
      echo -e "\n\nOpção inválida, tente novamente."
      ;;
  esac
done
