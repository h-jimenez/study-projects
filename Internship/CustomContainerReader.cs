using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using System.IO;
using UnityEditor;
using UnityEditor.Animations;
using UnityEngine.WSA;
using System.Linq;
using UnityEditorInternal;



public class CustomContainerReader : MonoBehaviour
{


    [MenuItem("Files/ Read Files")]
    private static void ReadFiles()
    {

            int _count = 0;
            
            string containerDirectoryPath = "Assets/CustomData"; // C:/Users/Legen/Desktop/PythonScript/output
            foreach (var filePath in Directory.GetFiles(containerDirectoryPath, "*.aap"))
            {
                ReadCustomContainer(filePath);
                _count++;
               
            }
        Debug.Log("Writing and importing" + " " + _count + " " + ".fbx and .wav files..." + "\n" + "This can take a while...");

        

        

    }

    [MenuItem("Files/ Extract Animations")]
    private static void ExtractAnimation()
    {
        string[] folders = { "Assets/DataFolder/FBX" };
        string[] objectsGUID = AssetDatabase.FindAssets("t:Object", folders);

        //Nombre d'objects dans le folder FBX
        Debug.Log(objectsGUID.Length);


        Object[] allObjects = new Object[objectsGUID.Length];

        int i = 0;

        string[] allObjectsPath = new string[objectsGUID.Length];

        //On récupère les chemins
        foreach (string objectGUID in objectsGUID)
        {
            var path = AssetDatabase.GUIDToAssetPath(objectGUID);
            allObjectsPath[i] = path;
            Debug.Log(path);
            Object singleObject = AssetDatabase.LoadAssetAtPath<Object>(path);
            allObjects[i] = singleObject;
            i++;
        }

        foreach (var singleObject in allObjects)
        {
            var fbx = AssetDatabase.GetAssetPath(singleObject);
            if(fbx != null)
            {
                Debug.Log("Extracting" + " " + fbx + " " + "animation...");
            }
            else
            {
                Debug.Log("Nofbx");
            }
            var directory = "Assets/DataFolder/Animations";
            //Path.GetDirectoryName(fbx);
            CreateAnim(fbx, directory);
        }
    }
    


    static void ReadCustomContainer(string filePath)
    {
        if (!File.Exists(filePath))
        {
            Debug.LogError("File not found: " + filePath);
            return;
        }

        using (BinaryReader reader = new BinaryReader(File.Open(filePath, FileMode.Open)))
        {
            // Read the FBX file
            string fbxIdentifier = new string(reader.ReadChars(3));
            if (fbxIdentifier != "FBX")
            {
                Debug.LogError("Invalid FBX identifier");
                return;
            }
            else
            {
                int fbxLength = reader.ReadInt32();
                byte[] fbxData = reader.ReadBytes(fbxLength);
                string fbxPath = Path.Combine("Assets/DataFolder/FBX", Path.GetFileNameWithoutExtension(filePath) + ".fbx");
                File.WriteAllBytes(fbxPath, fbxData);
            }

            // Read the WAV file
            string wavIdentifier = new string(reader.ReadChars(3));
            if (wavIdentifier != "WAV")
            {
                Debug.LogError("Invalid WAV identifier");
                return;
            }
            int wavLength = reader.ReadInt32();
            byte[] wavData = reader.ReadBytes(wavLength);
            string wavPath = Path.Combine("Assets/DataFolder/Audios", Path.GetFileNameWithoutExtension(filePath) + ".wav");
            File.WriteAllBytes(wavPath, wavData);

        }
    }
    static void CreateAnim(string fbx, string target)
    {
        var fileName = Path.GetFileNameWithoutExtension(fbx);
        var filePath = $"{target}/{fileName}.anim";
        AnimationClip src = AssetDatabase.LoadAssetAtPath<AnimationClip>(fbx);
        if (src != null) 
        {
            Debug.Log(src);
            AnimationClip temp = new AnimationClip();
            EditorUtility.CopySerialized(src, temp);
            AssetDatabase.CreateAsset(temp, filePath);
            AssetDatabase.SaveAssets();
        }
        else
        {
            Debug.Log("No animations was found for this .fbx");
            return;
        }
       
    }
    [MenuItem("Files/Create Animation Controller")]
    static void CreateAnimController()
    {
        string AnimFolder = "Assets/DataFolder/AnimationController";
        string[] folders = { "Assets/DataFolder/Animations" };
        string[] animationsGUID = AssetDatabase.FindAssets("t:AnimationClip", folders);
        Debug.Log("Number of animations found : " + animationsGUID.Count());


        foreach(string animationGUID in animationsGUID)
        {
            var path = AssetDatabase.GUIDToAssetPath(animationGUID);
                //Debug.Log(path);
            string fileName = Path.GetFileNameWithoutExtension(path);
            AnimationClip AnimClips = AssetDatabase.LoadAssetAtPath<AnimationClip>(path);
            var controller = UnityEditor.Animations.AnimatorController.CreateAnimatorControllerAtPath(AnimFolder+"/"+fileName+".controller");
            controller.AddParameter("AnimTime", UnityEngine.AnimatorControllerParameterType.Float);
            var rootStateMachine = controller.layers[0].stateMachine;
            var state = rootStateMachine.AddState(fileName);
            state.motion = AnimClips;
            state.timeParameterActive = true;

            if (controller != null)
            {
                Debug.Log("Animation Controller created for" + fileName + ".anim");
            }
            else
            {
                Debug.Log("An error occured during the process...");
            }
            
        }

    }

    


}
