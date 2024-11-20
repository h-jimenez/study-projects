import os
import sys
import struct
from pydub import AudioSegment
import numpy as np
from statistics import mean
from math import sqrt
import matplotlib.pyplot as plt
from fbx import * 
import scipy.io
import scipy.io.wavfile
import json


def create_custom_container(fbx_file, wav_file,json_file, output_file):
    with open(fbx_file, 'rb') as fbx, open(wav_file, 'rb') as wav, open(json_file, 'rb') as json:
        fbx_data = fbx.read()
        wav_data = wav.read()
        json_data = json.read()
        
        with open(output_file, 'wb') as output:
            # Write FBX data
            output.write(b'FBX')
            output.write(struct.pack('I', len(fbx_data)))
            output.write(fbx_data)
            
            # Write WAV data
            output.write(b'WAV')
            output.write(struct.pack('I', len(wav_data)))
            output.write(wav_data)

            # Write JSON data
            output.write(b'JSON')
            output.write(struct.pack('I', len(json_data)))
            output.write(json_data)

def calculate_rms(samples):
    max_abs_value = max(abs(sample) for sample in samples)
    normalized_samples = [sample / max_abs_value for sample in samples]
    rms = np.sqrt(np.mean(np.square(normalized_samples)))
    
    return rms

def readwav(wav_input):
    audio = AudioSegment.from_file(wav_input)
    
    num_channels = audio.channels
    sample_rate = audio.frame_rate
    sample_width = audio.sample_width
    
    # Conversion en mono (1 canal)
    audio = audio.set_channels(1)

    # Obtention des échantillons audio en int16
    samples = np.array(audio.get_array_of_samples())
    


    # Affichage des informations sur les échantillons
    min_value = np.min(samples)
    max_value = np.max(samples)
    mean_value = np.mean(samples)

    print(f"Fréquence d'échantillonage : {sample_rate}")
    print(f"Sample_width : {sample_width}")
    print(f"Valeur minimale des échantillons : {min_value}")
    print(f"Valeur maximale des échantillons : {max_value}")
    print(f"Moyenne des échantillons : {mean_value}")
    
    num_samples = len(samples)
    print(f"Nombre de samples : {num_samples}")
    
    durationaudio = num_samples / sample_rate
    print(f"Durée de l'audio : {durationaudio}")
    
    # Calculer la durée totale de l'audio en secondes
    duration = len(samples) / audio.frame_rate

    # Créer un tableau de temps en secondes pour le tracé
    time = np.linspace(0, duration, num=len(samples))

    # Plot de la forme d'onde
    plt.figure(figsize=(10, 4))
    plt.plot(time, samples)
    plt.title('Forme d\'onde de l\'audio')
    plt.xlabel('Temps (s)')
    plt.ylabel('Amplitude')
    plt.grid(True)
    plt.show()
    
    rms = calculate_rms(samples)
    print(f"Niveau RMS : {rms}")

    #barycentre spectral 

def readwavfile(wav_info) :
    myWavfile = wav_info
    
    sampleRate, audioBuffer = scipy.io.wavfile.read(myWavfile)
    
    duration = len(audioBuffer)/sampleRate
    time = np.arange(0,duration,1/sampleRate)

    moyenneLeft = mean(audioBuffer[:,0])
    print(f"Moyenne du channel gauche : {moyenneLeft}")
    maxLeft = max(audioBuffer[:,0])
    print(f"Maximum du channel gauche : {maxLeft}")
    minLeft = min(audioBuffer[:,0])
    print(f"Minimum du channel gauche : {minLeft}")
    moyenneRight = mean(audioBuffer[:,1])
    print(f"Moyenne du channel droit : {moyenneRight}")
    maxRight = max(audioBuffer[:,1])
    print(f"Maximum du channel droit : {maxRight}")
    minRight = min(audioBuffer[:,1])
    print(f"Minimum du channel droit : {minRight}")

    plt.plot(time,audioBuffer)
    plt.xlabel('Time')
    plt.ylabel('amplitude')
    plt.title(myWavfile)
    #plt.show()
    return audioBuffer,sampleRate


def initialization_fbx_object():
    fbxmanager = FbxManager.Create()
    ios = FbxIOSettings.Create(fbxmanager, IOSROOT)
    fbxmanager.SetIOSettings(ios)
    return fbxmanager, ios

def load_scene(fbxmanager, filepath):
    importer = FbxImporter.Create(fbxmanager,"")
    if not importer.Initialize(filepath,-1,fbxmanager.GetIOSettings()):
        print("Error: Unable to initialize importer")
        return None

    scene = FbxScene.Create(fbxmanager,"Scene")
    if not importer.Import(scene):
        print("Error: Unable to import scene")
        return None
    importer.Destroy()
    return scene

def get_animation_data(scene):

    # Initialisation du tableau de données
    animation_data = {}

    _3dVector = []
    timeVector = []
    # Nombre d'animations
    anim_stack_count = scene.GetSrcObjectCount(FbxCriteria.ObjectType(FbxAnimStack.ClassId))

    
    for i in range(anim_stack_count):
        anim_stack = scene.GetSrcObject(FbxCriteria.ObjectType(FbxAnimStack.ClassId), i)
        anim_stack_name = anim_stack.GetName()
        scene.SetCurrentAnimationStack(anim_stack)
        print(f"Animation Stack: {anim_stack_name}")

        anim_layer_count = anim_stack.GetMemberCount(FbxCriteria.ObjectType(FbxAnimLayer.ClassId))

        # Boucle sur chaque layer de l'animation
        for j in range(anim_layer_count):
            anim_layer = anim_stack.GetMember(FbxCriteria.ObjectType(FbxAnimLayer.ClassId), j)
            print(f"  Animation Layer: {anim_layer.GetName()}")

            node_count = scene.GetMemberCount(FbxCriteria.ObjectType(FbxNode.ClassId))

            # Boucle sur chaque nodes
            for k in range(node_count):
                node = scene.GetMember(FbxCriteria.ObjectType(FbxNode.ClassId), k)
                node_name = node.GetName()
                coord = ["X", "Y", "Z"]
                
                #On recupère un vecteur 3D _3dVector[x,y,z] et un vecteur temps timeVector pour l'animation de la main droite : 
                if "RightHand" in node_name :
                    for value in coord :
                        print(f"Valeur de translation pour la coordonnées {value}")
                        anim_curve = node.LclTranslation.GetCurve(anim_layer, value)

                        if anim_curve:
                            print(f"    Node: {node.GetName()} - Translation Animation Data:")
                            
                            node_coord = f"{node_name}_{value}"
                            print(node_coord)
                            if node_coord not in animation_data :
                                animation_data[node_coord] = []
                            # Boucle sur chaque key_index
                        
                                for key_index in range(anim_curve.KeyGetCount()):
                                    key_time = anim_curve.KeyGetTime(key_index).GetSecondDouble()
                                    key_value = anim_curve.KeyGetValue(key_index)
                                    animation_data[node_coord].append(key_value)
                                    if key_time not in timeVector:
                                        timeVector.append(key_time)
                        _3dVector.append(animation_data[node_coord])
                        
            #for node_name, keyframes in animation_data.items():
             #                       print(f"Node: {node_name}")
              #                      for key_time, key_value in keyframes :
               #                        print(f"Key Time : {key_time}, Key Value : {key_value}")
    checking = {0,1,2}
    for v in checking :
        if len(_3dVector[v]) != len(timeVector):
            print("Error: Vectors are not of the same length")
    return _3dVector, timeVector
    
def calculateSpeed(_3dVector, timeVector) :
    _3dVitesse = [[],[],[]]
    magnitudeVector = []
    for i in range(len(timeVector)-1):
        dt = timeVector[i+1] - timeVector[i]
        for dim in range(3):
            vectVitesse = (_3dVector[dim][i+1] - _3dVector[dim][i] ) / dt
            _3dVitesse[dim].append(vectVitesse)

    #Le dernier index est supprimé dans le calcul de la vitesse, à ne pas oublier !!!!
    return _3dVitesse

def calculateQOM(_3dVitesse):

    magnitudeVector = []
    for i in range(len(_3dVitesse[0])):
        # Calcule de la norme du vecteur vitesse pour chaque instant
        magnitudeCalc = sqrt(_3dVitesse[0][i]**2 + _3dVitesse[1][i]**2 + _3dVitesse[2][i]**2)
        magnitudeVector.append(magnitudeCalc)
        
    moyenneVit_x = sum(_3dVector[0]) / len(_3dVector[0])
    moyenneVit_y = sum(_3dVector[1]) / len(_3dVector[1])
    moyenneVit_z = sum(_3dVector[2]) / len(_3dVector[2])

    #print(_3dVector[0])
    print(f"Vitesse moyenne selon la coordonnées x : {abs(moyenneVit_x)} ")    
    print(f"Vitesse moyenne selon la coordonnées y : {abs(moyenneVit_y)} ") 
    print(f"Vitesse moyenne selon la coordonnées z : {abs(moyenneVit_z)} ") 
    
    moyenneMagnitude = mean(magnitudeVector)
    print(f"score de QoM : {moyenneMagnitude}")

    moyenneVit = [moyenneVit_x,moyenneVit_y,moyenneVit_z]
    
    return magnitudeVector,moyenneVit,moyenneMagnitude

def plotgraph(magnitudeVector,timeVector):
    
   

    # Plot de la forme d'onde
    plt.figure(figsize=(10, 4))
    plt.plot(timeVector[:-1], magnitudeVector)
    plt.title('Tracé de la quantité de mouvement de la main droite en fonction du temps')
    plt.xlabel('Temps (s)')
    plt.ylabel('QoM(m/s)')
    plt.grid(True)
    plt.show()                          
                        
                    

def barycentreSpectral(audioBuffer,sampleRate):
    audio_buffer = np.array(audioBuffer)
    
    # Calcul de la fft
    fft_buffer = np.fft.fft(audio_buffer)

    num_samples = len(audio_buffer)
    frequencies = np.fft.fftfreq(num_samples,d=1/sampleRate)

    #Module au carré de la fft
    energy_spectral = np.abs(fft_buffer[:,0]+fft_buffer[:,1])**2

    spectral_centroid = np.sum(frequencies*energy_spectral)/np.sum(energy_spectral)
    print(f"Barycentre spectral : {spectral_centroid}")
    return spectral_centroid
            

def jsonwriter(output_data,output_json):
    


    output_directory = r'D:\ETUDIANTS\2023-2024\STAGE Mocap\Dev\PythonScript\output'
    output_path = os.path.join(output_directory, "output" + '.json')



    data = {
      "semantic_data": {
            "Gesture" : "Gesture",
            "Action" : "Action",
            "bodyPart" : "Membre",
            "Description" : "Description",
            "Modificators" : "Modificators",
            },
        "signal_data" : {
            "Audio" : {
                "centralSpectroid" : output_data[4]
                },
            "Movement" : {
                "QoM" : output_data[0],
                "MeanVitx" : output_data[1],
                "MeanVity" : output_data[2],
                "MeanVitz" : output_data[3]
                }
            }
        }
    with open(output_json,'w') as json_file:
        json.dump(data,json_file,indent=10)
    print("Data is written in output.json")



def concatenate_output(moyenneMagnitude,moyenneVit,spectral_centroid):

    output_data = [None]*6

    # Data movement 
    

    output_data[0] = moyenneMagnitude
    output_data[1] = moyenneVit[0]
    output_data[2] = moyenneVit[1]
    output_data[3] = moyenneVit[2]
    output_data[4] = spectral_centroid
  
    return output_data




def concatenateallfiles():
    input_directory = r'D:\ETUDIANTS\2023-2024\STAGE Mocap\Dev\PythonScript\input'
    output_directory = r'D:\ETUDIANTS\2023-2024\STAGE Mocap\Dev\PythonScript\output'
    
    if not os.path.exists(output_directory):
        os.makedirs(output_directory)

    # Get list of all .fbx and .wav files in the input directory
    fbx_files = [f for f in os.listdir(input_directory) if f.endswith('.fbx')]
    wav_files = [f for f in os.listdir(input_directory) if f.endswith('.wav')]
    json_files = [f for f in os.listdir(input_directory)if f.endswith('.json')]

    # Create a set of base names (without extensions) for .fbx and .wav files
    fbx_base_names = {os.path.splitext(f)[0] for f in fbx_files}
    wav_base_names = {os.path.splitext(w)[0] for w in wav_files}
    json_base_names = {os.path.splitext(u)[0] for u in json_files}

    # Find matching base names
    matching_base_names1 = fbx_base_names.intersection(wav_base_names) 
    matching_base_names2 = fbx_base_names.intersection(json_base_names)
    matching_base_names = matching_base_names1.intersection(matching_base_names2)
    

    for base_name in matching_base_names:
        fbx_path = os.path.join(input_directory, base_name + '.fbx')
        wav_path = os.path.join(input_directory, base_name + '.wav')
        json_path = os.path.join(input_directory, base_name + '.json')
        output_file = os.path.join(output_directory, base_name + '.aap')
        create_custom_container(fbx_path, wav_path, json_path, output_file)





if __name__ == '__main__':

    



    #FBX RELATED OPERATIONS
    fbxmanager, ios = initialization_fbx_object()
    scene = load_scene(fbxmanager,filepath)
    if scene : 
        _3dVector,timeVector = get_animation_data(scene)
        _3dVitesse = calculateSpeed(_3dVector,timeVector)
        magnitudeVector,moyenneVit,moyenneMagnitude = calculateQOM(_3dVitesse)
        #plotgraph(magnitudeVector,timeVector)
    
    #AUDIO RELATED OPERATIONS
    audioBuffer,sampleRate = readwavfile(wav_info)
    spectral_centroid = barycentreSpectral(audioBuffer,sampleRate)

    #RETRIEVE ALL DATA 
    output_data = concatenate_output(moyenneMagnitude,moyenneVit,spectral_centroid)
    output_json = os.path.join(input_directory,'Raphael_TapeBaguetteEtParle.json')
    jsonwriter(output_data,output_json)
    
    #CREATE CUSTOM CONTAINER .AAP
    concatenateallfiles()







