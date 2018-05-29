import googlemaps
import requests
import json
import urllib

send_url = 'http://freegeoip.net/json'
r = requests.get(send_url)
j = json.loads(r.text)
coord = (j['latitude'],j['longitude'])

gmaps = googlemaps.Client(key="AIzaSyDEBDjOK6Df_VbK82_FsvCGdrCUFxpTqig")
reverse_result = gmaps.reverse_geocode(coord)

facultad = gmaps.geocode("Facultad de Ciencias, Plaza Caídos, s/n, 37008 Salamanca")
coord2 = (facultad[0]['geometry']['location']['lat'],facultad[0]['geometry']['location']['lng'])

direcciones = gmaps.directions(coord, coord2, region="es", language="es")

instruccion = direcciones[0]['legs'][0]['steps'][0]['html_instructions'];
distancia = direcciones[0]['legs'][0]['steps'][0]['distance']['text'];

if ("oeste" in instruccion or "izquierda" in instruccion):
	print("izquierda " + distancia)
elif ("este" in instruccion or "derecha" in instruccion):
	print("derecha " + distancia)
elif ("recto" in instruccion):
	print("recto " + distancia)
