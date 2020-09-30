import requests
import json
import time

url_nodemcu = 'http://192.168.1.8/data'

while True:
    response = requests.get(url_nodemcu)
    
    if response.status_code == 200:
        try:
            data = response.json()
            temperature = data.get('temperature')
            humidity = data.get('humidity')
            luminosity = data.get('luminosity')
            hora = time.strftime('%Y-%m-%d %H:%M:%S', time.localtime())

            new_data = "\n"+hora+";"+str(temperature) + ";" + str(humidity)+ ";" +str(luminosity)

            arquivo = open('dados.txt', 'r')

            conteudo = arquivo.readlines()

            conteudo.append(new_data)
        
            arquivo = open('dados.txt', 'w')
            arquivo.writelines(conteudo)
            arquivo.close()
        
            time.sleep(60)
        except:
            print('erro')
            
            time.sleep(50)