# Sistema Bancário SPICA — C++

Projeto desenvolvido como atividade avaliativa da disciplina **INF101 — Programação de Computadores I**, do Centro Universitário de Viçosa (UNIVIÇOSA).

O trabalho corresponde à **Etapa 3 — Implementação de um Sistema de Registro e Gestão de Contas Bancárias**, cujo objetivo principal é aprimorar o sistema desenvolvido nas etapas anteriores, implementando o armazenamento permanente dos dados por meio de arquivos em C++.

## Sobre o projeto

O Sistema Bancário SPICA é uma aplicação executada em terminal que permite realizar operações básicas de gerenciamento de contas bancárias.

Nesta etapa, o principal aprimoramento foi a implementação da persistência de dados, permitindo que as contas cadastradas, os saldos e o histórico de movimentações sejam preservados mesmo após o encerramento do programa.

Para isso, foram utilizados recursos da biblioteca `fstream`, com leitura e gravação de arquivos de texto, integrados aos arrays utilizados para armazenar os dados durante a execução.

## Funcionalidades implementadas

O sistema disponibiliza as seguintes operações:

- **Cadastro de contas:** registro do número da conta, nome do cliente, CPF, tipo de conta, saldo inicial e situação (ativa ou inativa).
- **Listagem de contas:** exibição das contas cadastradas e de suas respectivas informações.
- **Consulta de contas:** localização de uma conta pelo número.
- **Depósitos:** atualização do saldo e registro da movimentação.
- **Saques:** verificação de saldo disponível, atualização do saldo e registro da movimentação.
- **Transferências:** transferência de valores entre duas contas, com atualização dos saldos de origem e destino.
- **Extrato bancário:** consulta do histórico de depósitos e saques associados a uma conta.

## Persistência de dados — Etapa 3

O principal requisito desta etapa foi garantir que as informações do sistema não fossem perdidas ao encerrar sua execução.

Para atender a esse requisito, foram implementados dois arquivos de armazenamento:

### 1. contas.txt

Armazena os dados das contas bancárias:

- Número da conta;
- Nome do cliente;
- CPF;
- Tipo da conta;
- Saldo;
- Situação da conta.

Os campos são separados por ponto e vírgula (`;`).

### 2. movimentacoes.txt

Armazena o histórico das operações financeiras:

- Número da conta;
- Tipo da movimentação;
- Valor movimentado.

Os códigos utilizados são:

| Código | Operação |
|:---:|---|
| D | Depósito / Entrada |
| S | Saque / Saída |

Nas transferências, são registrados dois lançamentos: uma saída na conta de origem e uma entrada na conta de destino.

### Funcionamento do armazenamento

O sistema realiza o carregamento dos arquivos durante a inicialização, recuperando os registros e reconstruindo os arrays utilizados pelo programa.

Durante a execução, as operações que alteram os dados atualizam os respectivos arquivos.

A gravação é realizada percorrendo os arrays e registrando novamente os dados armazenados, preservando as informações das execuções anteriores.

Caso os arquivos ainda não existam, o programa inicia normalmente, permitindo a utilização do sistema desde a primeira execução.

## Recursos e conceitos de C++ utilizados

- Manipulação de arquivos com `fstream`;
- `ifstream` e `ofstream`;
- Leitura de linhas com `getline()`;
- Verificação de abertura de arquivos com `is_open()`;
- Conversão de dados com `stoi()` e `stof()`;
- Tratamento de exceções com `try/catch`;
- Manipulação de strings com `string` e `stringstream`;
- Arrays e variáveis globais;
- Funções para modularização das operações;
- Estruturas condicionais e de repetição;
- Enumeração (`enum`) para organização das opções do menu;
- Formatação de saída com `iomanip`.

## Organização das principais funções

| Função | Responsabilidade |
|---|---|
| `carregarContas()` | Recupera as contas armazenadas em arquivo. |
| `salvarContas()` | Grava os dados das contas em `contas.txt`. |
| `carregarMovimentacoes()` | Recupera o histórico de movimentações. |
| `salvarMovimentacoes()` | Grava o histórico em `movimentacoes.txt`. |
| `dadosConta()` | Realiza o cadastro de novas contas. |
| `listarContas()` | Exibe as contas cadastradas. |
| `ConsultarContasPeloNumero()` | Consulta uma conta específica. |
| `Deposito()` | Realiza depósitos. |
| `Saque()` | Realiza saques. |
| `Transferencia()` | Realiza transferências entre contas. |
| `ExibirExtrato()` | Apresenta o histórico de movimentações. |
| `MenuPrincipal()` | Controla a interação do usuário com o sistema. |

## Limites de armazenamento

A implementação utiliza arrays com capacidade previamente definida:

- Até 100 contas bancárias;
- Até 500 movimentações financeiras.

## Como executar

É necessário possuir um compilador compatível com C++.

Com o GCC ou MinGW instalado, compile o programa utilizando:

```bash
g++ Sistema-Bancario3.cpp -o sistema-bancario -std=c++11
```

Execute no terminal:

**Windows:**
```bash
sistema-bancario.exe
```

**Linux:**
```bash
./sistema-bancario
```

Os arquivos `contas.txt` e `movimentacoes.txt` são criados no diretório de execução quando os dados são gravados.

Para preservar os registros, mantenha esses arquivos no mesmo diretório de execução do programa.

## Contexto acadêmico

**Instituição:** Centro Universitário de Viçosa — UNIVIÇOSA  
**Disciplina:** INF101 — Programação de Computadores I  
**Professores:** Anderson R. Lamas e Vanderlea Queiroz  
**Atividade:** Trabalho — Sistema de Registro e Gestão de Contas Bancárias  
**Etapa:** 3  
**Natureza:** Atividade avaliativa

Esta implementação tem como finalidade aplicar os conceitos de programação estudados na disciplina, especialmente a manipulação de arquivos em C++, a organização por funções e a integração entre armazenamento temporário em memória e armazenamento permanente em arquivos.

---

**Projeto acadêmico desenvolvido em C++ como parte do processo de aprendizagem em programação de computadores.**
