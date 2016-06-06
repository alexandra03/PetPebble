Pebble.addEventListener('ready', function() {
  // PebbleKit JS is ready!
  console.log('PebbleKit JS ready!');
  
  // Request completed successfully
  function success(pos) {
    var dict = {
      'PebbleLatitude': pos.coords.latitude,
      'PebbleLongitude': pos.coords.longitude,
    };

    // Send the object to C
    Pebble.sendAppMessage(dict, function() {
      console.log('Message sent successfully: ' + JSON.stringify(dict));
    }, function(e) {
      console.log('Message failed: ' + JSON.stringify(e));
    });    
  }

  // Request did not complete successfully
  function error(err) {
    if(err.code == err.PERMISSION_DENIED) {
      console.log('Location access was denied by the user.');  
    } else {
      console.log('location error (' + err.code + '): ' + err.message);
    }
  }

  var options = {
    enableHighAccuracy: true,
    maximumAge: 10000,
    timeout: 10000
  };

  // Request current position
  navigator.geolocation.getCurrentPosition(success, error, options);
});