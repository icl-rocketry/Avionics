from flask import Flask
from flask import jsonify
from flask import request
from flask_pymongo import PyMongo

app = Flask(__name__)

app.config['MONGO_DBNAME'] = 'restdb'
app.config['MONDO_URI'] = 'mongodb://localhost:27017/restdb'

mongo = PyMongo(app)

