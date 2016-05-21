#!/bin/bash

javac -cp ".:IARRC_Sim_api.jar" geometry/*.java
javac -cp ".:IARRC_Sim_api.jar" graphics/*.java
javac -cp ".:IARRC_Sim_api.jar" gui/*.java
javac -cp ".:IARRC_Sim_api.jar" mapping/*.java
