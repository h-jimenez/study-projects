using System.Collections;
using System.Collections.Generic;
using Unity.VisualScripting;
using UnityEngine;

public class AudioAnimPlayer : MonoBehaviour
{

    private float AudioSampleRate;
    private float _calcTime;
    private float _animTime;
    private bool _isPlaying;

    public float _loopStart;
    public float _loopEnd;
    public bool _loopEnabled;

    public float _speedModifier;

    private float _clipDuration;

    AudioSource _audioSource;
    Animator _animator;

    public float AudioTime;
    public float AudioTime_percent;

    public AudioClip _audioClip;
    public Avatar Avatar;

    void Start()
    {
        AudioSampleRate = AudioSettings.outputSampleRate;

       
        gameObject.AddComponent<AudioSource>();
        _audioSource = GetComponent<AudioSource>();
        

        
        gameObject.AddComponent<Animator>();
        _animator = GetComponent<Animator>();

        _animator.runtimeAnimatorController = Resources.Load(gameObject.name) as RuntimeAnimatorController;
        _animator.applyRootMotion = true;
        _animator.cullingMode = AnimatorCullingMode.CullUpdateTransforms;
        _animator.avatar = Avatar;


        _audioSource.clip = _audioClip;

        _audioSource.Play();
        _audioSource.loop = true;
        

        _clipDuration = _audioClip.length;

        _loopStart = 0;
        _loopEnd = _clipDuration;

        _speedModifier = 1;
    }


    void Update()
    {

       //Synchronisation avec l'audio à l'aide d'un paramètre float dans l'animation controller 

       GetComponent<Animator>().SetFloat("AnimTime", _animTime);

       _isPlaying = _audioSource.isPlaying;


        if (_loopEnabled== true)
        {
            if(_audioSource.time > _loopEnd)
            {
                _audioSource.time = _loopStart;
            }
        }

        AudioTime = _audioSource.time;
        AudioTime_percent = AudioTime / _audioClip.length;

        
        _audioSource.pitch = _speedModifier;


    }



    private void OnAudioFilterRead(float[] data, int channels)
    {


        //On travaille ici en secondes


        if(_loopEnabled == true)
        {
            if(_calcTime >= _loopEnd)
            {
                _calcTime = _loopStart;
            }
        }
        else
        {
            if (_calcTime >= _clipDuration)
            {
                //Reset de l'animation
                _calcTime = 0;
            }
        }

        if(_isPlaying == true)
        {
            _calcTime += _speedModifier * ((data.Length / channels) / AudioSampleRate);
        }
       
        //On travaille ici en pourcentage de durée de l'audio
        _animTime = _calcTime / _clipDuration;

    }


    public void Unpause()
    {
        _audioSource.UnPause();
    }

    public void Pause()
    {
        _audioSource.Pause();
    }

    public void LoopAnim()
    {
       
        
    }

    public void SetSpeed()
    {
        
    }
}
