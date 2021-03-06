﻿using UnityEngine;
using System.Collections;
using System.Collections.Generic;

public class MouseMoveLock : MonoBehaviour {


    private bool bValidPosition;
    private Tile startPoint;
    private BuildingGenerator bGenerator;
    private PathFinder pFinder;
    private TileManager tm;
    private bool bFirstClick;

    // Use this for initialization
    void Start () {
        bValidPosition = false;
        startPoint = null;
        bGenerator = GameObject.Find("Controller").GetComponent<BuildingGenerator>();
        pFinder = GameObject.Find("Map").GetComponent<PathFinder>();
        tm = GameObject.Find("Map").GetComponent<TileManager>();
        bFirstClick = true;
	}


    // Returns true if the building has been placed
    public bool BuildingMouseMove(GameObject building, bool placeBuilding, bool bRoad, GameObject road, bool bShovel, bool bRepairTool) 
    {
        bValidPosition = false;
        GameObject tileBuilding = null;
        float raycastLength = 1000;
        Ray ray = Camera.main.ScreenPointToRay(Input.mousePosition);
        RaycastHit[] hits = Physics.RaycastAll(ray, raycastLength);

        if (!bRoad)
            startPoint = null;

        if (hits.Length > 0) {

            for (int i = 0; i < hits.Length; i++)
            {
                if (hits[i].collider.gameObject.tag == "Tile" && hits[i].point.y>=0.12)
                {

                    var obj = hits[i].collider;
                    Building buildcomp = building.GetComponent<Building>();

                    float posX = obj.gameObject.transform.position.x;
                    float posY = building.transform.position.y;
                    float posZ = obj.gameObject.transform.position.z;


                    if (isEven(buildcomp.sizeX))
						posX += 0.5f;

                    if (isEven(buildcomp.sizeZ))
						posZ += 0.5f;

                    building.transform.position = new Vector3(posX, posY, posZ);

                    if (!bRepairTool)
                        bValidPosition = checkValidPosition(buildcomp, hits[i].collider.gameObject.GetComponent<Tile>().posX, hits[i].collider.gameObject.GetComponent<Tile>().posZ);
                    else
                    {
                        tileBuilding = GetTileBuilding(hits[i].collider.gameObject.GetComponent<Tile>());
                        if (tileBuilding != null)
                            bValidPosition = checkValidRepairPosition(tileBuilding);
                    }

                    Renderer rend = building.GetComponent<Renderer>();
                    if (placeBuilding && bValidPosition)
                    {
                        if (!bRoad)
                        {
                            if (!bShovel)
                            {
                                if (!bRepairTool)
                                {
                                    tm.buildingList.Add(building);

                                    rend.material.SetColor("_SpecColor", Color.black);
                                    setOcuppiedTile(buildcomp, hits[i].collider.gameObject.GetComponent<Tile>().posX, hits[i].collider.gameObject.GetComponent<Tile>().posZ);
                                    building.transform.position = new Vector3(posX, posY - 0.5f, posZ);

                                    // Notify building it has been placed if some component wants to do something
                                    building.SendMessage("buildingBuilt", SendMessageOptions.DontRequireReceiver);

                                    return true;
                                }
                                else
                                {
                                    tileBuilding.GetComponent<BuildingDestructor>().AvoidDestruction();
                                    Destroy(building);
                                    return true;
                                }
                            }
                            else
                            {
                                setFreeTile(buildcomp, hits[i].collider.gameObject.GetComponent<Tile>().posX, hits[i].collider.gameObject.GetComponent<Tile>().posZ);
                                Destroy(building);
                                return true;
                            }
                        }
                        else {
                            if (startPoint == null)
                            {
                                bFirstClick = false;
                                startPoint = hits[i].collider.gameObject.GetComponent<Tile>();
                                bGenerator.CreateRoad(pFinder.GetRoadPath(startPoint, hits[i].collider.gameObject.GetComponent<Tile>()), road);
                                return false;
                            }
                            else {
                                List<Tile> path = pFinder.GetRoadPath(startPoint, hits[i].collider.gameObject.GetComponent<Tile>());
                                bGenerator.CreateRoad(path, road);
                                foreach (Tile tile in path) {
                                    setOcuppiedTile(buildcomp, tile.posX, tile.posZ);
                                }
                                DestroyObject(building);
                                bFirstClick = true;
                                startPoint = null;
                                bGenerator.newRoad.Clear();
                                return true;
                            }
                        }
                    }
                    else {
                        if (bValidPosition)
                        {
                            rend.material.SetColor("_SpecColor", Color.green);
                            //Debug.Log("broad es "+bRoad+" bFirstclick es "+bFirstClick);
                            if (bRoad && !bFirstClick) {
                                //Debug.Log("HOLAAAA");
                                bGenerator.CreateRoad(pFinder.GetRoadPath(startPoint, hits[i].collider.gameObject.GetComponent<Tile>()), road);
                            }
                        }
                        else
                            rend.material.SetColor("_SpecColor", Color.red);
                    }

                }
            }
        }
        return false;
    }

    GameObject GetTileBuilding(Tile tile)
    {
        Building buildcomp;
        
        foreach (GameObject building in tm.buildingList) {

            buildcomp = building.GetComponent<Building>();

            foreach (Tile buildingTile in buildcomp.getTiles()) {

                if (buildingTile == tile) {
                    return building;
                }
            }
        }

        return null;
    }

    bool checkValidRepairPosition(GameObject building) {
        return building.GetComponent<BuildingDestructor>().CheckIncomingDestruction();
    }

    bool checkValidPosition(Building building, uint x, uint z)
    {

        Tile[,] tiles = tm.tiles;
        uint mapSizeX = tm.getSizeX();
        uint mapSizeZ = tm.getSizeZ();
        int liminfX = 0, liminfZ = 0, limsupX = 0, limsupZ = 0;

        liminfX = (int)x - (int)building.sizeX / 2;
        limsupX = (int)x + (int)building.sizeX / 2;
        liminfZ = (int)z - (int)building.sizeZ / 2;
        limsupZ = (int)z + (int)building.sizeZ / 2;

        if (isEven(building.sizeX)) // Even
            ++liminfX;
        if (isEven(building.sizeZ)) // Even
            ++liminfZ;

        if (liminfX < 0 || liminfZ < 0 || limsupX >= mapSizeX || limsupZ >= mapSizeZ)
            return false;

        for (int i = liminfX; i <= limsupX; ++i) {
            for (int j = liminfZ; j <= limsupZ; ++j)
            {
                //Debug.Log("Tile "+i+" "+j+" tipo "+tiles[i,j].type+" tipo buscado "+building.validTileType);
                if ((tiles[i, j].buildingType != TileType.EMPTY && tiles[i, j].buildingType != TileType.RUINS) || (tiles[i, j].type != building.validTileType))
                    return false;
            }
        }
        return true;
    }

    public void setOcuppiedTile(Building building, uint x, uint z)
    {
        Tile[,] tiles = tm.tiles;
        int liminfX = 0, liminfZ = 0, limsupX = 0, limsupZ = 0;

        liminfX = (int)x - (int)building.sizeX / 2;
        limsupX = (int)x + (int)building.sizeX / 2;
        liminfZ = (int)z - (int)building.sizeZ / 2;
        limsupZ = (int)z + (int)building.sizeZ / 2;

        if (isEven(building.sizeX)) // Even
            ++liminfX;
        if (isEven(building.sizeZ)) // Even
            ++liminfZ;

        for (int i = liminfX; i <= limsupX; ++i)
        {
            for (int j = liminfZ; j <= limsupZ; ++j)
            {
                building.addTile(tiles[i, j]);
                tiles[i,j].buildingType = building.BuildingType;
            }
        }
    }

    public void setFreeTile(Building building, uint x, uint z)
    {
        Tile[,] tiles = tm.tiles;
        int liminfX = 0, liminfZ = 0, limsupX = 0, limsupZ = 0;

        liminfX = (int)x - (int)building.sizeX / 2;
        limsupX = (int)x + (int)building.sizeX / 2;
        liminfZ = (int)z - (int)building.sizeZ / 2;
        limsupZ = (int)z + (int)building.sizeZ / 2;

        if (isEven(building.sizeX)) // Even
            ++liminfX;
        if (isEven(building.sizeZ)) // Even
            ++liminfZ;

        for (int i = liminfX; i <= limsupX; ++i)
        {
            for (int j = liminfZ; j <= limsupZ; ++j)
            {
                tiles[i, j].buildingType = TileType.EMPTY;
                tiles[i, j].type = tm.getMapTileType(i,j);
                tiles[i, j].init();
            }
        }
    }

    bool isEven(float num)
    {
        return (num % 2) == 0;
    }
}
