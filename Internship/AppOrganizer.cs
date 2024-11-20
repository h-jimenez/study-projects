using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEditor;
using System.IO;
using UnityEngine.WSA;

/*
public class AppOrganizer : MonoBehaviour
{

    [MenuItem("Examples/Create Animation")]
    static void CreateAnimation()
    {
        string[] guids;
        string[] folders = AssetDatabase.GetSubFolders("Assets/DataFolder");

        foreach (string folder in folders)
        {
            //Debug.Log(folder);
        }


        guids = AssetDatabase.FindAssets("t:AnimationClip", folders);


        foreach (string guid in guids)
        {
            //Debug.Log("Animation:" + AssetDatabase.GUIDToAssetPath(guid));
            AnimationClip _animclip = (AnimationClip)AssetDatabase.LoadAssetAtPath(guid, typeof(AnimationClip));
            // AssetDatabase.CreateAsset(_animclip, "Assets" + _animclip.name);
            Debug.Log(_animclip.name);
        }




    }
*/
public class AnimationExtractor : MonoBehaviour
{
    [MenuItem("Assets/Extract Animation")]
    private static void ExtractAnimation()
    {
        string[] folders = {"Assets/DataFolder/FBX"};
        string[] objectsGUID = AssetDatabase.FindAssets("t:Object",folders);

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
            var directory = "Assets/DataFolder/Animations";
                //Path.GetDirectoryName(fbx);
            CreateAnim(fbx, directory);
        }
    }

    static void CreateAnim(string fbx, string target)
    {
        var fileName = Path.GetFileNameWithoutExtension(fbx);
        var filePath = $"{target}/{fileName}.anim";
        AnimationClip src = AssetDatabase.LoadAssetAtPath<AnimationClip>(fbx);
        AnimationClip temp = new AnimationClip();
        EditorUtility.CopySerialized(src, temp);
        AssetDatabase.CreateAsset(temp, filePath);
        AssetDatabase.SaveAssets();
    }
}