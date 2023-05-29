import os
from flask import Flask,jsonify,request
from currency_converter import CurrencyConverter
c = CurrencyConverter()

app = Flask(__name__)

@app.route('/convertemoeda/<valor>',methods=['GET'])
def converte_moeda(valor):
    print(valor)
    try:
        new_valor = round(float(valor),2)
        return jsonify({
            'hostname': os.uname()[1],
            'conversao': {
            'real': round(new_valor,2),
            'dolar': round(c.convert(new_valor, 'BRL', 'USD'),2),
            'euro': round(c.convert(new_valor, 'BRL', 'EUR'),2)}
            })
    except:
        return 'valor precisa ser do tipo inteiro ou float, ex: 5.30'

@app.route("/")
def hello():
    return "Hello World!"

if __name__ == "__main__":
    app.run(host='0.0.0.0',debug=True)
