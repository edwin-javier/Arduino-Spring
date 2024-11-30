async function fetchParkingData() {
    try {
        const response = await fetch('/parkingData');
        const parkingData = await response.json();
        renderParkingLot(parkingData);
    } catch (error) {
        console.error("Error obteniendo los datos del parqueo", error);
    }
}

function renderParkingLot(parkingData) {
    // Espacios regulares
    parkingData.disponibles.forEach((estado, index) => {
        const spotElement = document.getElementById(`regular-spot-${index + 1}`);
        const carElement = document.getElementById(`car-regular-${index + 1}`);
        
        // Cambiar el estado del rectángulo
        const rect = spotElement.firstElementChild; // Obtener el rectángulo

        if (estado === 1) {
            rect.setAttribute('class', 'available'); // Cambiar clase a disponible
            carElement.style.display = 'none'; // Ocultar coche
        } else {
            rect.setAttribute('class', 'occupied'); // Cambiar clase a ocupado
            carElement.style.display = 'block'; // Mostrar coche
        }
    });

    // Espacios reservados
    parkingData.disponiblesR.forEach((estado, index) => {
        const spotElement = document.getElementById(`reserved-spot-${index + 1}`);
        const carElement = document.getElementById(`car-reserved-${index + 1}`);

        // Cambiar el estado del rectángulo
        const rect = spotElement.firstElementChild; // Obtener el rectángulo

        if (estado === 1) {
            rect.setAttribute('class', 'reserved'); // Cambiar clase a reservado
            carElement.style.display = 'none'; // Ocultar coche
        } else {
            rect.setAttribute('class', 'reserved-occupied'); // Cambiar clase a reservado ocupado
            carElement.style.display = 'block'; // Mostrar coche
        }
    });
}

setInterval(fetchParkingData, 5000);

fetchParkingData();






