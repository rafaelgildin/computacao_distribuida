# Aluno: Rafael Avraham Gildin Acherboim - TIA: 31940242

Execução das 3 máquinas na AWS.
Uma delas com o servidor nginx para balancear as chamadas às 2 máquinas do back.
Na parte superior, há um notebook que realizou 2 chamadas, de forma que uma delas foi direcionada ao webserver 1 e a outra ao webserver 2, tudo através da chamada ao ip do balanceador.
Na parte inferior há 3 terminais, um para cada máquina ec2 na AWS. Da esquerda pra direita, temos o balanceador, o webserver 1 e o webserver 2.
Ambos os webservers estão rodando o código do repositório em python na porta 5000.
- ![Alt text](imgs/execucao.png?raw=true "foto")

Execução das 3 máquinas na AWS.
- ![Alt text](imgs/aws_ec2s.png?raw=true "foto")

ifconfigs:
- ![Alt text](imgs/ifconfigs.png?raw=true "foto")

- Balanceador: o arquivo de configuração do nginx da máquina balanceador mostrando os IPs das máquinas (localizados em /etc/nginx/conf.d/load-balancer.conf)
- ![Alt text](imgs/configuracao_nginx.png?raw=true "foto")

Códigos utilizados

```
************** balanceador:
52.91.245.165
export key="rafael-teste-01.pem"\
&& ssh -i $key ubuntu@ec2-52-91-245-165.compute-1.amazonaws.com

export name=balanceador-rafael-31940242 \
&& sudo hostnamectl set-hostname $name \
&& hostname

sudo apt update && sudo apt install nginx

sudo rm /etc/nginx/conf.d/load-balancer.conf \
&& sudo nano /etc/nginx/conf.d/load-balancer.conf

upstream backend {
    server 44.203.164.109:5000; # webserver1-Rafael-31940242
    server 3.93.6.43:5000; # webserver2-Rafael-31940242
}
server {
    listen 80;
    server_name 52.91.245.165; # balanceador-Rafael-31940242
    location / {
        proxy_pass http://backend;
    }
}

sudo service nginx restart

************** webserver1:
44.203.164.109
export key="rafael-teste-01.pem" \
&& ssh -i $key ubuntu@ec2-44-203-164-109.compute-1.amazonaws.com	

export name=webserver1-rafael-31940242 \
&& sudo hostnamectl set-hostname $name \
&& hostname

************** webserver2:
3.93.6.43
export key="rafael-teste-01.pem" \
&& ssh -i $key ubuntu@ec2-3-93-6-43.compute-1.amazonaws.com

export name=webserver2-rafael-31940242 \
&& sudo hostnamectl set-hostname $name \
&& hostname

-------------------- run python from github for the 2 webservers
sudo apt install python3-pip
git clone https://github.com/profmariomenezes/compdis-webservice-balanceamento-wba_rgildin.git
cd compdis-webservice-balanceamento-wba_rgildin
pip install -r requirements.txt

-------------------- install apache (NOT USED)
sudo apt update && sudo apt install apache2
sudo rm /var/www/html/index.html
sudo nano /var/www/html/index.html

<html>
<h1> Seja bem vindo ao site de Rafael - TIA 31940242</h1>
<h1> webserver1-Rafael-31940242</h1>
<HTML>

```

# Professor
[![Review Assignment Due Date](https://classroom.github.com/assets/deadline-readme-button-24ddc0f5d75046c5622901739e7c5dd533143b0c8e959d652212380cedb1ea36.svg)](https://classroom.github.com/a/kgpTODK_)
[![Open in Codespaces](https://classroom.github.com/assets/launch-codespace-7f7980b617ed060a017424585567c406b6ee15c891e84e1186181d67ecf80aa0.svg)](https://classroom.github.com/open-in-codespaces?assignment_repo_id=10990039)
# compdist-webservice-balanceamento

## Atividade de Laboratório

Crie uma infraestrutura no Amazon EC2 ou Azure para rodar um serviço web (webservice) com balanceamento de carga, em nuvem.
Você deve implementar um mecanismo de balanceamento de cargas utilizando o servidor web Ngnix; (1 servidor front-end balanceador de carga)
Os servidores Web backend rodarão diretamente o webservice escutando na porta 5000  (2 servidores)

Disponibilize, no seu servidor, um serviço de conversão de real para dólar e euro como um webservice RESTFul: o usuário informa o valor em real, seu serviço obtém (de alguma API externa) a taxa do dólar e do euro para venda e devolve o valor em dólares e euros correspondentes ao valor em real fornecido, com saída JSON.  

A função deve se chamar: convertemoeda

A URL deverá ser  `<http://nome_da_maquina.dominio/convertemoeda/<VALOR>`

E o resultado deverá ser em JSON:

```
{
   “conversao”: [
    “real”: <VALOR>,
    “dolar”: <VALOR_EM_DOLAR>,
    “euro”: <VALOR_EM_EURO>,
   ]
}
```

Você deve implementar seu webservice em Python com Flask (esta atividade deve ser feita desta forma; outras atividades poderão, eventualmente, ser em linguagem livre).

Exemplos/tutoriais:

- https://dzone.com/articles/restful-web-services-with-python-flask (bem simples, fácil de usar)
- https://opensource.com/article/17/3/writing-web-service-using-python-flask (mais complexo, mostra uma aplicação bem estruturada)
- https://programminghistorian.org/en/lessons/creating-apis-with-python-and-flask (bem completa, documentação detalhada)
- https://www.datascienceblog.net/post/programming/flask-api-development/ (mostra como documentar a API com Swagger, muito bom)
- https://realpython.com/flask-by-example-part-1-project-setup/ (quase um curso completo para uma aplicação bem completa e complexa)


O que entregar no GitHub: Todo o código, Um relatório em PDF com todos os prints da execução, captura de telas e detalhamento da implementação. 

No dia determinado pelo professor os grupos farão a apresentação/demonstração da execução do sistema em funcionamento.
