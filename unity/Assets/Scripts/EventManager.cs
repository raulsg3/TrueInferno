﻿using UnityEngine;
using System.Collections;

public class EventManager : MonoBehaviour {

    private TimeManager timeManager;
    private TileManager tileManager;

    //Event parameters
    public float destroyInterval;
    public float destroyDelay;
    public int destroyChance;

    // Event time controller
    private float remainingDestroyEventTime;

    private float previousTime;

	// Use this for initialization
	void Start () 
    {
        timeManager = GameObject.Find("TimeManager").GetComponent<TimeManager>();
        tileManager = GameObject.Find("Map").GetComponent<TileManager>();
        previousTime = 60 * timeManager.minutesTimeLimit;
        remainingDestroyEventTime = destroyInterval;
	}
	
	// Update is called once per frame
	void Update ()
    {
        // Update event timers
        remainingDestroyEventTime -= (previousTime - timeManager.remainingTime);

        // Check for event timers
        EventChecker();

        // Update timer
        previousTime = timeManager.remainingTime;
	}


    // Method for event controlling
    void EventChecker() 
    {
        if (remainingDestroyEventTime <= 0)
            BuildingDestructionEvent();
    }

    //Event Methods
    void BuildingDestructionEvent()
    {
        remainingDestroyEventTime = destroyInterval;
        bool eventSuccess = UnityEngine.Random.Range(1,100) <= destroyChance;
        if (eventSuccess)
        {
            eventSuccess = false;
            foreach (GameObject building in tileManager.buildingList)
            { 
                if (building.GetComponent<BuildingDestructor>()!=null)
                {
                    eventSuccess = true;
                    break;
                }
            }
        }
        
        if ( eventSuccess && tileManager.buildingList.Count > 0)
        {
            eventSuccess = false;
            BuildingDestructor targetBuildingDestructor;

            while (!eventSuccess)
            {
                targetBuildingDestructor = tileManager.buildingList[UnityEngine.Random.Range(0, tileManager.buildingList.Count)].GetComponent<BuildingDestructor>();
                if (targetBuildingDestructor != null)
                {
                    targetBuildingDestructor.DestroyBuilding(destroyDelay);
                    eventSuccess = true;
                }
            }
        }
    }
}
