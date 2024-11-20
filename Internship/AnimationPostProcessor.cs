using System.Collections;
using System.Collections.Generic;
using UnityEditor;
using UnityEngine;

public class AnimationPostProcessor : AssetPostprocessor
{
    ModelImporter modelImporter;
    private void OnPreprocessModel()
    {
        var modelImporter = assetImporter as ModelImporter;
        modelImporter.animationType = ModelImporterAnimationType.Human;


    }
}
