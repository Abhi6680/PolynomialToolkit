let chartInstance = null;

function evaluatePolynomial(coeffs, x) {
    let y = 0;
    for (let deg = 0; deg < coeffs.length; deg++) {
        y += coeffs[deg] * Math.pow(x, deg);
    }
    return y;
}

function renderPolynomialGraph(coeffs, containerId = 'polyChart') {
    const ctx = document.getElementById(containerId).getContext('2d');
    const xValues = [];
    const yValues = [];

    for (let x = -10; x <= 10; x += 0.2) {
        xValues.push(x.toFixed(1));
        yValues.push(evaluatePolynomial(coeffs, x));
    }

    if (chartInstance) {
        chartInstance.destroy();
    }

    chartInstance = new Chart(ctx, {
        type: 'line',
        data: {
            labels: xValues,
            datasets: [{
                label: 'P(x)',
                data: yValues,
                borderColor: '#2563eb',
                backgroundColor: 'rgba(37, 99, 235, 0.1)',
                borderWidth: 2.5,
                fill: true,
                pointRadius: 0
            }]
        },
        options: {
            responsive: true,
            plugins: {
                legend: { display: true }
            },
            scales: {
                x: { title: { display: true, text: 'x' } },
                y: { title: { display: true, text: 'P(x)' } }
            }
        }
    });
}

function handlePlot() {
    const input = document.getElementById('polyInput').value;
    renderPolynomialGraph([1, 2, 1]);
}