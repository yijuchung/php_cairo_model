// create a 5 gear car
$car = new Car(5);
print $car->getCurrentSpeed();  // prints '0'
$car->accelerate();
print $car->getCurrentSpeed(); // prints '5'
