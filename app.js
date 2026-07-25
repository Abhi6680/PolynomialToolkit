// Render Polynomial Graph dynamically using Chart.js
function plotPolynomial(coeffsArr) {
    const ctx = document.getElementById('polyChart').getContext('2d');
    const labels = [];
    const dataPoints = [];

    for (let x = -10; x <= 10; x += 0.2) {
        labels.push(x.toFixed(1));
        let y = 0;
        for (let i = 0; i < coeffsArr.length; i++) {
            y += coeffsArr[i] * Math.pow(x, i);
        }
        dataPoints.push(y);
    }

    if (window.myChart) {
        window.myChart.destroy();
    }

    window.myChart = new Chart(ctx, {
        type: 'line',
        data: {
            labels: labels,
            datasets: [{
                label: 'P(x)',
                data: dataPoints,
                borderColor: '#2563eb',
                backgroundColor: 'rgba(37, 99, 235, 0.1)',
                borderWidth: 2,
                fill: true,
                pointRadius: 0
            }]
        },
        options: {
            responsive: true,
            scales: {
                x: { title: { display: true, text: 'x' } },
                y: { title: { display: true, text: 'P(x)' } }
            }
        }
    });
}