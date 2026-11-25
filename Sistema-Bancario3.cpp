#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <exception>
#include <iomanip>

const int MAX_CONTAS = 100;
const int MAX_MOVIMENTACOES = 500;

// Arrays globais das contas
int numeroConta[MAX_CONTAS];
int tipoConta[MAX_CONTAS];
std::string nomeCliente[MAX_CONTAS];
std::string cpfCliente[MAX_CONTAS];
float saldoConta[MAX_CONTAS];
bool contaAtiva[MAX_CONTAS];
int quantidadeContas = 0;

// Arrays globais das movimentacoes
int contaMovimentacao[MAX_MOVIMENTACOES];
char tipoMovimentacao[MAX_MOVIMENTACOES];
float valorMovimentacao[MAX_MOVIMENTACOES];
int quantidadeMovimentacoes = 0;

// Formata um valor no padrao brasileiro: R$ 1.000,00
std::string formatarMoeda(float valor) {
  bool negativo = valor < 0;
  if (negativo) valor = -valor;

  long long centavos = (long long)(valor * 100 + 0.5f);
  long long inteiro = centavos / 100;
  int resto = (int)(centavos % 100);

  std::string digitos = std::to_string(inteiro);
  std::string comMilhar;
  int contador = 0;
  for (int i = (int)digitos.size() - 1; i >= 0; i--) {
    comMilhar = digitos[i] + comMilhar;
    contador++;
    if (contador % 3 == 0 && i > 0) comMilhar = "." + comMilhar;
  }

  std::string centavosTxt = (resto < 10 ? "0" : "") + std::to_string(resto);
  return std::string(negativo ? "-" : "") + "R$ " + comMilhar + "," + centavosTxt;
}

// Retorna "numero - nome" para exibir nas tabelas
std::string rotuloConta(int indice) {
  return std::to_string(numeroConta[indice]) + " - " + nomeCliente[indice];
}

void exibirMenu() {
  std::cout << "=== Sistema Bancario SPICA ===" << std::endl;
  std::cout << "1. Cadastrar Conta" << std::endl;
  std::cout << "2. Listar Contas" << std::endl;
  std::cout << "3. Consultar Contas pelo Numero" << std::endl;
  std::cout << "4. Realizar Deposito" << std::endl;
  std::cout << "5. Realizar Saque" << std::endl;
  std::cout << "6. Realizar Transferencia" << std::endl;
  std::cout << "7. Exibir Extrato" << std::endl;
  std::cout << "0. Sair" << std::endl;
  std::cout << "Escolha uma opcao: ";
}

void salvarContas() {
  std::ofstream salvarArquivo("contas.txt");

  if (!salvarArquivo.is_open()) {
    std::cerr << "Erro ao abrir o arquivo para salvar contas." << std::endl;
    return;
  }

  for (int i = 0; i < quantidadeContas; i++) {
    salvarArquivo << numeroConta[i] << ";"
                  << nomeCliente[i] << ";"
                  << cpfCliente[i] << ";"
                  << tipoConta[i] << ";"
                  << saldoConta[i] << ";"
                  << contaAtiva[i] << std::endl;
  }

  salvarArquivo.close();
}

void salvarMovimentacoes() {
  std::ofstream salvarArquivo("movimentacoes.txt");

  if (!salvarArquivo.is_open()) {
    std::cerr << "Erro ao abrir o arquivo para salvar movimentacoes." << std::endl;
    return;
  }

  for (int i = 0; i < quantidadeMovimentacoes; i++) {
    salvarArquivo << contaMovimentacao[i] << ";"
                  << tipoMovimentacao[i] << ";"
                  << valorMovimentacao[i] << std::endl;
  }

  salvarArquivo.close();
}

void carregarContas() {
  std::ifstream carregarArquivo("contas.txt");

  if (!carregarArquivo.is_open()) {
    return; // Primeira execucao, arquivo ainda nao existe
  }

  quantidadeContas = 0;
  std::string linha;

  while (quantidadeContas < MAX_CONTAS && std::getline(carregarArquivo, linha)) {
    // Remove '\r' caso o arquivo tenha sido salvo no Windows
    if (!linha.empty() && linha.back() == '\r') linha.pop_back();

    // Ignora linhas vazias
    if (linha.empty()) continue;

    std::stringstream ss(linha);
    std::string campoNumero, campoNome, campoCpf, campoTipo, campoSaldo, campoAtiva;

    std::getline(ss, campoNumero, ';');
    std::getline(ss, campoNome, ';');
    std::getline(ss, campoCpf, ';');
    std::getline(ss, campoTipo, ';');
    std::getline(ss, campoSaldo, ';');
    std::getline(ss, campoAtiva, ';');

    // Ignora linhas incompletas
    if (campoNumero.empty() || campoTipo.empty() ||
        campoSaldo.empty() || campoAtiva.empty()) {
      continue;
    }

    try {
      int numero = std::stoi(campoNumero);
      int tipo = std::stoi(campoTipo);
      float saldo = std::stof(campoSaldo);

      numeroConta[quantidadeContas] = numero;
      nomeCliente[quantidadeContas] = campoNome;
      cpfCliente[quantidadeContas] = campoCpf;
      tipoConta[quantidadeContas] = tipo;
      saldoConta[quantidadeContas] = saldo;
      contaAtiva[quantidadeContas] = (campoAtiva == "1");

      quantidadeContas++;
    } catch (const std::exception&) {
      std::cerr << "Aviso: linha invalida ignorada em contas.txt: " << linha << std::endl;
    }
  }

  carregarArquivo.close();
}

void carregarMovimentacoes() {
  std::ifstream carregarArquivo("movimentacoes.txt");

  if (!carregarArquivo.is_open()) {
    return; // Primeira execucao, arquivo ainda nao existe
  }

  quantidadeMovimentacoes = 0;
  std::string linha;

  while (quantidadeMovimentacoes < MAX_MOVIMENTACOES && std::getline(carregarArquivo, linha)) {
    if (!linha.empty() && linha.back() == '\r') linha.pop_back();

    if (linha.empty()) continue;

    std::stringstream ss(linha);
    std::string campoConta, campoTipo, campoValor;

    std::getline(ss, campoConta, ';');
    std::getline(ss, campoTipo, ';');
    std::getline(ss, campoValor, ';');

    if (campoConta.empty() || campoTipo.empty() || campoValor.empty()) {
      continue;
    }

    try {
      int conta = std::stoi(campoConta);
      float valor = std::stof(campoValor);

      contaMovimentacao[quantidadeMovimentacoes] = conta;
      tipoMovimentacao[quantidadeMovimentacoes] = campoTipo[0];
      valorMovimentacao[quantidadeMovimentacoes] = valor;

      quantidadeMovimentacoes++;
    } catch (const std::exception&) {
      std::cerr << "Aviso: linha invalida ignorada em movimentacoes.txt: " << linha << std::endl;
    }
  }

  carregarArquivo.close();
}

void ConsultarContasPeloNumero(int numero) {
  bool encontrada = false;

  for (int i = 0; i < quantidadeContas; i++) {
    if (numeroConta[i] == numero) {
      std::cout << "=== Dados da Conta ===" << std::endl;
      std::cout << "Numero: " << numeroConta[i] << std::endl;
      std::cout << "Nome: " << nomeCliente[i] << std::endl;
      std::cout << "CPF: " << cpfCliente[i] << std::endl;
      std::cout << "Tipo: " << (tipoConta[i] == 1 ? "Corrente" : "Poupanca") << std::endl;
      std::cout << "Saldo: " << formatarMoeda(saldoConta[i]) << std::endl;
      std::cout << "Situacao: " << (contaAtiva[i] ? "Ativa" : "Inativa") << std::endl;
      encontrada = true;
      break;
    }
  }

  if (!encontrada) {
    std::cout << "Conta nao encontrada." << std::endl;
  }
}

void Deposito(int numero, float valor) {
  if (quantidadeMovimentacoes >= MAX_MOVIMENTACOES) {
    std::cout << "Limite de movimentacoes atingido." << std::endl;
    return;
  }

  bool encontrada = false;

  for (int i = 0; i < quantidadeContas; i++) {
    if (numeroConta[i] == numero) {
      if (!contaAtiva[i]) {
        std::cout << "Conta inativa. Deposito nao realizado." << std::endl;
        return;
      }
      saldoConta[i] += valor;
      std::cout << "Deposito de " << formatarMoeda(valor) << " realizado com sucesso." << std::endl;
      std::cout << "Novo saldo: " << formatarMoeda(saldoConta[i]) << std::endl;

      // Registrar movimentacao
      contaMovimentacao[quantidadeMovimentacoes] = numero;
      tipoMovimentacao[quantidadeMovimentacoes] = 'D';
      valorMovimentacao[quantidadeMovimentacoes] = valor;
      quantidadeMovimentacoes++;

      salvarContas();
      salvarMovimentacoes();
      encontrada = true;
      break;
    }
  }

  if (!encontrada) {
    std::cout << "Conta nao encontrada." << std::endl;
  }
}

void Saque(int numero, float valor) {
  if (quantidadeMovimentacoes >= MAX_MOVIMENTACOES) {
    std::cout << "Limite de movimentacoes atingido." << std::endl;
    return;
  }

  bool encontrada = false;

  for (int i = 0; i < quantidadeContas; i++) {
    if (numeroConta[i] == numero) {
      if (!contaAtiva[i]) {
        std::cout << "Conta inativa. Saque nao realizado." << std::endl;
        return;
      }
      if (saldoConta[i] < valor) {
        std::cout << "Saldo insuficiente. Saque nao realizado." << std::endl;
        return;
      }
      saldoConta[i] -= valor;
      std::cout << "Saque de " << formatarMoeda(valor) << " realizado com sucesso." << std::endl;
      std::cout << "Novo saldo: " << formatarMoeda(saldoConta[i]) << std::endl;

      // Registrar movimentacao
      contaMovimentacao[quantidadeMovimentacoes] = numero;
      tipoMovimentacao[quantidadeMovimentacoes] = 'S';
      valorMovimentacao[quantidadeMovimentacoes] = valor;
      quantidadeMovimentacoes++;

      salvarContas();
      salvarMovimentacoes();
      encontrada = true;
      break;
    }
  }

  if (!encontrada) {
    std::cout << "Conta nao encontrada." << std::endl;
  }
}

void Transferencia(int numeroContaOrigem, int numeroContaDestino, float valor) {
  // A transferencia registra 2 movimentacoes
  if (quantidadeMovimentacoes + 2 > MAX_MOVIMENTACOES) {
    std::cout << "Limite de movimentacoes atingido." << std::endl;
    return;
  }

  int indiceOrigem = -1;
  int indiceDestino = -1;

  for (int i = 0; i < quantidadeContas; i++) {
    if (numeroConta[i] == numeroContaOrigem) indiceOrigem = i;
    if (numeroConta[i] == numeroContaDestino) indiceDestino = i;
  }

  if (indiceOrigem == -1) {
    std::cout << "Conta de origem nao encontrada." << std::endl;
    return;
  }
  if (indiceDestino == -1) {
    std::cout << "Conta de destino nao encontrada." << std::endl;
    return;
  }
  if (!contaAtiva[indiceOrigem]) {
    std::cout << "Conta de origem inativa. Transferencia nao realizada." << std::endl;
    return;
  }
  if (!contaAtiva[indiceDestino]) {
    std::cout << "Conta de destino inativa. Transferencia nao realizada." << std::endl;
    return;
  }
  if (saldoConta[indiceOrigem] < valor) {
    std::cout << "Saldo insuficiente na conta de origem. Transferencia nao realizada." << std::endl;
    return;
  }

  float saldoAnteriorOrigem = saldoConta[indiceOrigem];
  float saldoAnteriorDestino = saldoConta[indiceDestino];

  saldoConta[indiceOrigem] -= valor;
  saldoConta[indiceDestino] += valor;

  // Registrar movimentacao de saida na conta de origem
  contaMovimentacao[quantidadeMovimentacoes] = numeroContaOrigem;
  tipoMovimentacao[quantidadeMovimentacoes] = 'S';
  valorMovimentacao[quantidadeMovimentacoes] = valor;
  quantidadeMovimentacoes++;

  // Registrar movimentacao de entrada na conta de destino
  contaMovimentacao[quantidadeMovimentacoes] = numeroContaDestino;
  tipoMovimentacao[quantidadeMovimentacoes] = 'D';
  valorMovimentacao[quantidadeMovimentacoes] = valor;
  quantidadeMovimentacoes++;

  salvarContas();
  salvarMovimentacoes();

  std::cout << std::endl;
  std::cout << "Transferencia de " << formatarMoeda(valor) << " realizada com sucesso." << std::endl;

  // Tabela de saldos
  std::cout << std::endl;
  std::cout << std::left
            << std::setw(25) << "Conta"
            << std::setw(18) << "Saldo anterior"
            << "Saldo atualizado" << std::endl;
  std::cout << std::string(61, '-') << std::endl;
  std::cout << std::left
            << std::setw(25) << rotuloConta(indiceOrigem)
            << std::setw(18) << formatarMoeda(saldoAnteriorOrigem)
            << formatarMoeda(saldoConta[indiceOrigem]) << std::endl;
  std::cout << std::left
            << std::setw(25) << rotuloConta(indiceDestino)
            << std::setw(18) << formatarMoeda(saldoAnteriorDestino)
            << formatarMoeda(saldoConta[indiceDestino]) << std::endl;

  // Tabela do historico registrado
  std::cout << std::endl;
  std::cout << "Historico:" << std::endl;
  std::cout << std::left
            << std::setw(10) << "Conta"
            << std::setw(8) << "Tipo"
            << "Valor" << std::endl;
  std::cout << std::string(30, '-') << std::endl;
  std::cout << std::left
            << std::setw(10) << numeroContaOrigem
            << std::setw(8) << 'S'
            << formatarMoeda(valor) << std::endl;
  std::cout << std::left
            << std::setw(10) << numeroContaDestino
            << std::setw(8) << 'D'
            << formatarMoeda(valor) << std::endl;
  std::cout << std::endl;
}

void ExibirExtrato(int numero) {
  bool contaExiste = false;

  for (int i = 0; i < quantidadeContas; i++) {
    if (numeroConta[i] == numero) {
      contaExiste = true;
      break;
    }
  }

  if (!contaExiste) {
    std::cout << "Conta nao encontrada." << std::endl;
    return;
  }

  std::cout << "=== Extrato da Conta " << numero << " ===" << std::endl;
  bool temMovimentacao = false;

  for (int i = 0; i < quantidadeMovimentacoes; i++) {
    if (contaMovimentacao[i] == numero) {
      if (!temMovimentacao) {
        std::cout << std::left
                  << std::setw(10) << "Conta"
                  << std::setw(8) << "Tipo"
                  << "Valor" << std::endl;
        std::cout << std::string(30, '-') << std::endl;
      }
      std::cout << std::left
                << std::setw(10) << contaMovimentacao[i]
                << std::setw(8) << tipoMovimentacao[i]
                << formatarMoeda(valorMovimentacao[i]) << std::endl;
      temMovimentacao = true;
    }
  }

  if (!temMovimentacao) {
    std::cout << "Nenhuma movimentacao encontrada para esta conta." << std::endl;
  }
}

void listarContas() {
  if (quantidadeContas == 0) {
    std::cout << "Nenhuma conta cadastrada." << std::endl;
    return;
  }

  std::cout << "=== Lista de Contas ===" << std::endl;
  std::cout << std::left
            << std::setw(8) << "Conta"
            << std::setw(22) << "Nome"
            << std::setw(16) << "CPF"
            << std::setw(10) << "Tipo"
            << std::setw(16) << "Saldo"
            << "Situacao" << std::endl;
  std::cout << std::string(80, '-') << std::endl;

  for (int i = 0; i < quantidadeContas; i++) {
    std::cout << std::left
              << std::setw(8) << numeroConta[i]
              << std::setw(22) << nomeCliente[i]
              << std::setw(16) << cpfCliente[i]
              << std::setw(10) << (tipoConta[i] == 1 ? "Corrente" : "Poupanca")
              << std::setw(16) << formatarMoeda(saldoConta[i])
              << (contaAtiva[i] ? "Ativa" : "Inativa")
              << std::endl;
  }
}

void dadosConta() {
  if (quantidadeContas >= MAX_CONTAS) {
    std::cout << "Limite de contas atingido." << std::endl;
    return;
  }

  int numero, tipo, ativa;
  float saldo;
  std::string nome, cpf;

  std::cout << "Digite o numero da conta: ";
  std::cin >> numero;

  std::cout << "Digite o tipo da conta (1 - Corrente, 2 - Poupanca): ";
  std::cin >> tipo;

  std::cout << "Digite o nome do cliente: ";
  std::cin.ignore(10000, '\n'); // Limpar o buffer do teclado
  std::getline(std::cin, nome);

  std::cout << "Digite o CPF do cliente: ";
  std::getline(std::cin, cpf);

  std::cout << "Digite o saldo da conta: ";
  std::cin >> saldo;

  std::cout << "A conta esta ativa? (1 - Sim, 0 - Nao): ";
  std::cin >> ativa;

  // Se alguma leitura numerica falhou, cancela sem gravar lixo no arquivo
  if (std::cin.fail()) {
    std::cin.clear();
    std::cin.ignore(10000, '\n');
    std::cout << "Entrada invalida. Cadastro cancelado." << std::endl;
    return;
  }

  numeroConta[quantidadeContas] = numero;
  tipoConta[quantidadeContas] = tipo;
  nomeCliente[quantidadeContas] = nome;
  cpfCliente[quantidadeContas] = cpf;
  saldoConta[quantidadeContas] = saldo;
  contaAtiva[quantidadeContas] = (ativa == 1);

  quantidadeContas++;
  salvarContas();

  std::cout << "Conta cadastrada com sucesso." << std::endl;
}

enum acoes 
{
  CADASTRAR_CONTA = 1,
  LISTAR_CONTAS = 2,
  CONSULTAR_CONTA = 3,
  DEPOSITO = 4,
  SAQUE = 5,
  TRANSFERENCIA = 6,
  EXTRATO = 7,
  SAIR = 0
};


void MenuPrincipal() {
  int numero;
  int opcao;
  float valor;
  int numeroContaOrigem, numeroContaDestino;

  do {
    exibirMenu();
    std::cin >> opcao;

    // Evita loop infinito se o usuario digitar algo que nao e numero
    if (std::cin.fail()) {
      std::cin.clear();
      std::cin.ignore(10000, '\n');
      std::cout << "Opcao invalida. Tente novamente." << std::endl;
      opcao = -1;
      continue;
    }

    switch (opcao) {

      case CADASTRAR_CONTA:
        dadosConta();
        break;
      case LISTAR_CONTAS:
        listarContas();
        break;
      case CONSULTAR_CONTA:
        std::cout << "Digite o numero da conta a ser consultada: ";
        std::cin >> numero;
        ConsultarContasPeloNumero(numero);
        break;
      case DEPOSITO:
        std::cout << "Digite o numero da conta para deposito: ";
        std::cin >> numero;
        std::cout << "Digite o valor do deposito: ";
        std::cin >> valor;
        Deposito(numero, valor);
        break;
      case SAQUE:
        std::cout << "Digite o numero da conta para saque: ";
        std::cin >> numero;
        std::cout << "Digite o valor do saque: ";
        std::cin >> valor;
        Saque(numero, valor);
        break;
      case TRANSFERENCIA:
        std::cout << "Digite o numero da conta de origem: ";
        std::cin >> numeroContaOrigem;
        std::cout << "Digite o numero da conta de destino: ";
        std::cin >> numeroContaDestino;
        std::cout << "Digite o valor da transferencia: ";
        std::cin >> valor;
        Transferencia(numeroContaOrigem, numeroContaDestino, valor);
        break;
      case EXTRATO:
        std::cout << "Digite o numero da conta para exibir o extrato: ";
        std::cin >> numero;
        ExibirExtrato(numero);
        break;
      case SAIR:
        std::cout << "Saindo do sistema..." << std::endl;
        break;
      default:
        std::cout << "Opcao invalida. Tente novamente." << std::endl;
    }
  } while (opcao != SAIR);
}

int main() {
  carregarContas();
  carregarMovimentacoes();

  MenuPrincipal();

  return 0;
}