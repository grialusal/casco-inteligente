import googlemaps
#import requests
#import json

gmaps = googlemaps.Client(key="AIzaSyDEBDjOK6Df_VbK82_FsvCGdrCUFxpTqig")

#send_url = 'http://freegeoip.net/json'
#r = requests.get(send_url)
#j = json.loads(r.text)
#coord = (j['latitude'],j['longitude'])

plaza = gmaps.geocode("Universidad de Salamanca. Facultad de Economía y Empresa, Campus Miguel de Unamuno, Paseo Francisco Tomás y Valiente, s/n, 37007 Salamanca")
coord = (plaza[0]['geometry']['location']['lat'], plaza[0]['geometry']['location']['lng'])

facultad = gmaps.geocode("Mercadona, Av. Padre Ignacio Ellacuría, s/n, 37008 Salamanca")
coord2 = (facultad[0]['geometry']['location']['lat'], facultad[0]['geometry']['location']['lng'])

direcciones = gmaps.directions(coord, coord2, region="es", language="es", mode="bicycling")

f = open('instrucciones.txt', 'w')

for instruccion in direcciones[0]['legs'][0]['steps']:
	html = instruccion['html_instructions']
	distancia = str(instruccion['distance']['value']);
	tiempo = str(instruccion['duration']['value']);
	if "primera" in html:
		f.write("rotonda 1 " + distancia + " " + tiempo + "\n")
	elif "segunda" in html:
		f.write("rotonda 2 " + distancia + " " + tiempo + "\n")
	elif "tercera" in html:
		f.write("rotonda 3 " + distancia + " " + tiempo + "\n")
	elif "cuarta" in html:
		f.write("rotonda 4 " + distancia + " " + tiempo + "\n")
	elif "maneuver" in instruccion:
		if ("left" in instruccion['maneuver']):
			f.write("izquierda " + distancia + " " + tiempo + "\n")
		elif ("right" in instruccion['maneuver']):
			f.write("derecha " + distancia + " " + tiempo + "\n")
		elif ("straight" in instruccion['maneuver']):
			f.write("recto " + distancia + " " + tiempo + "\n")
	else:
		f.write("recto " + distancia + " " + tiempo + "\n")

f.close()
