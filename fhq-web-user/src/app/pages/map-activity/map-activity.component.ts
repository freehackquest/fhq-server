import { Component, OnInit, ChangeDetectorRef } from '@angular/core';
import { SpinnerService } from '../../services/spinner.service';
import { FhqService } from '../../services/fhq.service';
import Feature from 'ol/Feature.js';
import Map from 'ol/Map.js';
import View from 'ol/View.js';
import Point from 'ol/geom/Point.js';
import { Projection, transform } from 'ol/proj.js';
import { Tile as TileLayer, Vector as VectorLayer } from 'ol/layer.js';
import { Cluster, OSM, Vector as VectorSource } from 'ol/source.js';
import { Circle as CircleStyle, Fill, Stroke, Style, Text } from 'ol/style.js';

@Component({
  selector: 'app-map-activity',
  templateUrl: './map-activity.component.html',
  styleUrls: ['./map-activity.component.css']
})
export class MapActivityComponent implements OnInit {
  latitude: number = 18.5204;
  longitude: number = 73.8567;
  errorMessage: string = null;
  map: any;
  distance: number = 20;

  constructor(
    private _spinner: SpinnerService,
    private _cdr: ChangeDetectorRef,
    private _fhq: FhqService,
  ) {

  }

  ngOnInit() {
    const _data = {}
    this._spinner.show();
    this._fhq.api().getmap(_data)
    .done((r: any) => this.successResponse(r))
    .fail((err: any) => this.errorResponse(err));
  }

  successResponse(r: any) {
    console.log(r);
    this._spinner.hide();
    let features = [];
    r.data.forEach((p: any) => {
      const coordinates = [p.lng, p.lat];
      const point = new Point(transform(coordinates, 'EPSG:4326', 'EPSG:900913'));
      features.push(new Feature({
        geometry: new Point(transform(coordinates, 'EPSG:4326', 'EPSG:900913')),
        population: p.count
      }));
    });
    features.push(new Feature({
      geometry: new Point(transform([84.977987, 56.454026], 'EPSG:4326', 'EPSG:900913')),
      name: 'Null Island',
      population: 1,
    }));

    var source = new VectorSource({
      features: features
    });

    var clusterSource = new Cluster({
      distance: this.distance,
      source: source
    });

    var styleCache = {};
    var clusters = new VectorLayer({
      source: clusterSource,
      style: function(feature: any) {
        var size = feature.get('features').length;
        var style = styleCache[size];
        if (!style) {
          style = new Style({
            image: new CircleStyle({
              radius: 10,
              stroke: new Stroke({
                color: '#fff'
              }),
              fill: new Fill({
                color: '#3399CC'
              })
            }),
            text: new Text({
              text: size.toString(),
              fill: new Fill({
                color: '#fff'
              })
            })
          });
          styleCache[size] = style;
        }
        return style;
      }
    });

    var raster = new TileLayer({
      source: new OSM()
    });

    var map = new Map({
      layers: [raster, clusters],
      target: 'map',
      projection: new Projection("EPSG:900913"),
      displayProjection: new Projection("EPSG:4326"),
      view: new View({
        center: [0, 0],
        zoom: 2
      })
    });

    // if need chnage distance
    // clusterSource.setDistance(parseInt(distance.value, 10));
    this._cdr.detectChanges();
  }

  errorResponse(err: any) {
    this._spinner.hide();
    this.errorMessage = err.error;
    this._cdr.detectChanges();
    console.error(err);
  }

}
