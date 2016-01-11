﻿using UnityEngine;
using UnityEngine.UI;

using System.Collections;
using System;

public class ResourceManager : MonoBehaviour {

    /// <summary>
    /// Base gathering speed per minute and worker for both map resources.
    /// 
    /// This velocities will be affected for some other factors (usually
    /// incremented) as the game progresses.
    /// </summary>
    private const float RES_1_BASE_GATHERING_SPEED = 20.0f / 60.0f;
    private const float RES_2_BASE_GATHERING_SPEED = 12.0f  / 60.0f;

    /// <summary>
    /// Single resource increment per soul used.
    /// </summary>
    private const float COKE_PER_SOUL = 8;
    private const float CRUDE_PER_SOUL = 2;

    /// <summary>
    /// Current gathering speed per minute and worker for both map resources,
    /// already including all factors to be considered.
    /// </summary>
    private float res1GatheringSpeed = RES_1_BASE_GATHERING_SPEED;
    private float res2GatheringSpeed = RES_2_BASE_GATHERING_SPEED;

    /// <summary>
    /// Map resources game values.
    /// Float numbers will allow us to control them better and play with lower
    /// numbers, even though they will always be displayed as int numbers.
    /// </summary>
    private float res1 = 0; //"Mineral"
    private float res2 = 0; //"Gas"
    private float coke = 0;
    private float crude = 0;
    private float pure = 0;
    private float refined = 0;

    /// <summary>
    /// UI text elements where both resources game values are displayed.
    /// </summary>
    public Text res1Display; // mineral
    public Text res2Display; // gas
    public Text cokeDisplay; // coke
    public Text crudeDisplay; // crude
    public Text pureDisplay; // pure
    public Text refinedDisplay; // refined

    void Awake() {
        if (!res1Display || !res2Display || !cokeDisplay || !crudeDisplay || !pureDisplay || !refinedDisplay)
        {
            Debug.LogError("Map resources displays not assigned!");
        }
    }

    void Start () {
    }

    void Update() {
        //Updates both resources values being displayed
        res1Display.text = ((int)Math.Floor(res1)).ToString();
        res2Display.text = ((int)Math.Floor(res2)).ToString();
        cokeDisplay.text = ((int)Math.Floor(coke)).ToString();
        crudeDisplay.text = ((int)Math.Floor(crude)).ToString();
        pureDisplay.text = ((int)Math.Floor(pure)).ToString();
        refinedDisplay.text = ((int)Math.Floor(refined)).ToString();
    }

    /// <summary>
    /// Increments the value of the first map resource according to the number
    /// of workers and the time. Method to be called by every Resource 1
    /// gathering building.
    /// </summary>
    public void incRes1(int workers, float time) {
        res1 += res1GatheringSpeed * workers * time;
    }

    /// <summary>
    /// Increments the value of the second map resource according to the number
    /// of workers and the time. Method to be called by every Resource 2
    /// gathering building.
    /// </summary>
    public void incRes2(int workers, float time) {
        res2 += res2GatheringSpeed * workers * time;
    }

    public void burnSouls(int numSouls)
    {
        coke += numSouls * COKE_PER_SOUL;
        crude += numSouls * CRUDE_PER_SOUL;
    }

}
