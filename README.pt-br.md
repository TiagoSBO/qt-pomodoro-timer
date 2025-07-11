# 🍎Aplicativo Timer Pomodoro

[![Em Desenvolvimento](https://img.shields.io/badge/Status-Em%20Desenvolvimento-yellow)](https://shields.io/)
[![Netlify Status](https://api.netlify.com/api/v1/badges/cc0be1bf-2eae-4e81-9ced-627ba616499e/deploy-status)](https://app.netlify.com/projects/qtpomodoro-timer/deploys)

## Descrição

Pomodoro Timer é um projeto baseado na técnica pomodoro desenvolvida pelo autor [Francesco Cirillo](https://www-pomodorotechnique-com.translate.goog/the-pomodoro-technique-book/?_x_tr_sl=en&_x_tr_tl=pt&_x_tr_hl=pt&_x_tr_pto=tc). O objetivo deste projeto foi projetar um sistema focado em aumentar a produtividade e o foco.

O aplicativo opera na bandeja do sistema, fornecendo acesso discreto aos controles do cronômetro e às configurações. Os usuários podem personalizar a duração dos períodos de trabalho e pausas, configurar alertas sonoros e revisar o histórico de sessões por meio de logs detalhados.

## Tecnologias utilizadas

<img src="https://cdn.jsdelivr.net/gh/devicons/devicon@latest/icons/cplusplus/cplusplus-original.svg" width="30" height="30"/> <img src="https://cdn.jsdelivr.net/gh/devicons/devicon@latest/icons/python/python-original.svg" width="30" height="30" /> <img src="https://cdn.jsdelivr.net/gh/devicons/devicon@latest/icons/qt/qt-original.svg" width="30" height="30"/> <img src="https://cdn.jsdelivr.net/gh/devicons/devicon@latest/icons/cmake/cmake-original.svg" width="30" height="30" /> <img src="https://cdn.jsdelivr.net/gh/devicons/devicon@latest/icons/netlify/netlify-original.svg" width="30" height="30"/> <img src="https://cdn.jsdelivr.net/gh/devicons/devicon@latest/icons/sqlite/sqlite-original-wordmark.svg" width="40" height="30" /> 

## Funcionalidades

- **🔗 Integração com a Bandeja do Sistema:** O aplicativo roda minimizado na bandeja do sistema, mantendo seu espaço de trabalho limpo e organizado.
  - Minimizar para a bandeja  
  - Restaurar da bandeja  
  - Sair da bandeja  
  - Restaurar janela flutuante

- **⿻ Janela Flutuante:** Uma janela flutuante móvel que exibe o tempo do cronômetro. Esta janela pode ser ativada clicando no cronômetro ou pressionando o atalho **Ctrl + Shift + 0**.

- **⏲️ Temporizadores Personalizáveis:** Ajuste a duração de suas sessões de trabalho, pausas curtas e longas conforme sua preferência.
  - Número de Pomodoros concluídos  
  - Tempo de foco  
  - Horário de término da sessão

- **⏰ Alertas Sonoros:** Configure notificações sonoras personalizadas para alertar quando o cronômetro terminar, garantindo que você mantenha o foco.

- **⚙️ Configurações:** Ajuste vários aspectos do aplicativo, desde a duração dos cronômetros até preferências sonoras, garantindo uma experiência personalizada.

- **📈 Logs de Sessão:** Logs detalhados das suas sessões pomodoro são armazenados, permitindo que você acompanhe sua produtividade ao longo do tempo.

### Em breve:
- **🎨 Múltiplos Temas:** Vários temas para mudar a aparência do aplicativo.

### Demonstração

![funcionalidades (1)](https://github.com/user-attachments/assets/86bc6873-f589-42eb-8605-68def035ff2b)

**Janela flutuante:** Pode ser ativada com o atalho **Ctrl + Shift + 0** ou clicando no rótulo do cronômetro.

![float2 (1)](https://github.com/user-attachments/assets/7dccbef9-e0c1-428b-8a94-9607cb49ee7d)

**Janela de estatísticas:** Visualize e controle seu progresso com a tabela de estatísticas, filtre suas sessões por data personalizada ou por períodos.

![statswindow (1)](https://github.com/user-attachments/assets/df826a3e-5509-43d6-b2c5-a46546b34171)

## Guia de Instalação

Você pode instalar o aplicativo utilizando o instalador fornecido ou compilando a partir do código-fonte.

### Usando o Instalador

- Vá para a seção de [Releases](https://github.com/TiagoSBO/qt-pomodoro-timer/releases).
- Baixe a versão mais recente (arquivo *.exe).
- Execute o instalador e siga as instruções na tela.

### Compilando do Código-Fonte

Se preferir compilar o aplicativo a partir do código-fonte, siga estes passos:

**Clone o repositório:**
```bash
git clone https://github.com/TiagoSBO/qt-pomodoro-timer.
git cd qt-pomodoro-timer
```
**Instale as Dependências:**
Você vai precisar de:
- CMake
- Qt 6.x (ou Qt 5.15+)

> Certifique-se de adicionar qmake e cmake ao PATH do seu sistema

No Windows (usando Chocolatey):
```
bash choco install cmake qt6
```
No Linux:
```
sudo apt install cmake qt6-base-dev qt6-tools-dev-tools build-essential
```

**Compile o Aplicativo:**
Crie um diretório de build e use o CMake para configurar e compilar o projeto.
```
mkdir build
cd build
cmake ..
cmake --build .
```

**Execute o Aplicativo:**
Após uma compilação bem-sucedida, você pode executar o binário do aplicativo. A localização do executável dependerá da sua configuração de build e sistema operacional. Ele é normalmente encontrado dentro do diretório `build` ou em um subdiretório como `build/bin` ou `build/debug`.
```
./qt-pomodoro-timer
```

## Agradecimentos:

Para que este programa funcione como esperado, ele conta com projetos incríveis como:

- [QHotkey](https://github.com/Skycoder42/QHotkey) do usuário [Skycoder42](https://github.com/Skycoder42).
- [SingleApplication](https://github.com/itay-grudev/SingleApplication) do usuário [Itay Grudev](https://github.com/itay-grudev).

## Contribuindo

Contribuições são bem-vindas! Se você tiver sugestões, relatos de bugs ou quiser contribuir com código, fique à vontade para abrir uma issue ou enviar um pull request.

