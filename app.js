let chartInstance = null;

function evaluatePoly(expr, x) {
    let expanded = expr.replace(/\((x)\s*([\+\-])\s*(\d+(\.\d+)?)\)\^2/g, (match, varX, sign, val) => {
        let a = parseFloat(val) * (sign === '-' ? -1 : 1);
        let doubleA = 2 * a;
        let aSq = a * a;
        return `(x^2 ${doubleA >= 0 ? '+' : ''}${doubleA}x + ${aSq})`;
    });

    expanded = expanded.replace(/[\(\)]/g, '');

    let terms = expanded.match(/[\+\-]?\s*[^+\-]+/g) || [];
    let total = 0;

    terms.forEach(term => {
        term = term.trim().replace(/\s+/g, '');
        if (!term) return;

        let coef = 1;
        let power = 0;

        if (term.includes('x')) {
            let parts = term.split('x');
            if (parts[0] === '' || parts[0] === '+') coef = 1;
            else if (parts[0] === '-') coef = -1;
            else coef = parseFloat(parts[0]);

            if (parts[1] && parts[1].startsWith('^')) {
                power = parseFloat(parts[1].substring(1));
            } else {
                power = 1;
            }
        } else {
            coef = parseFloat(term);
            power = 0;
        }

        if (!isNaN(coef)) {
            total += coef * Math.pow(x, power);
        }
    });

    return total;
}

function evaluateDerivative(expr, x, h = 0.001) {
    return (evaluatePoly(expr, x + h) - evaluatePoly(expr, x - h)) / (2 * h);
}

function plotPolynomial() {
    const inputExpr = document.getElementById('polyInput').value;
    if (!inputExpr.trim()) return;

    const xValues = [];
    const yPoly = [];
    const yDeriv = [];

    for (let x = -5; x <= 5; x += 0.2) {
        let roundedX = Math.round(x * 100) / 100;
        xValues.push(roundedX);
        yPoly.push(evaluatePoly(inputExpr, roundedX));
        yDeriv.push(evaluateDerivative(inputExpr, roundedX));
    }

    const ctx = document.getElementById('polyChart').getContext('2d');

    if (chartInstance) {
        chartInstance.destroy();
    }

    chartInstance = new Chart(ctx, {
        type: 'line',
        data: {
            labels: xValues,
            datasets: [
                {
                    label: `f(x) = ${inputExpr}`,
                    data: yPoly,
                    borderColor: 'rgba(54, 162, 235, 1)',
                    borderWidth: 2,
                    fill: false,
                    tension: 0.1
                },
                {
                    label: `f'(x) (Derivative)`,
                    data: yDeriv,
                    borderColor: 'rgba(255, 99, 132, 1)',
                    borderWidth: 2,
                    borderDash: [5, 5],
                    fill: false,
                    tension: 0.1
                }
            ]
        },
        options: {
            responsive: true,
            maintainAspectRatio: false,
            scales: {
                x: { title: { display: true, text: 'x' } },
                y: { title: { display: true, text: 'y' } }
            }
        }
    });
}

window.onload = plotPolynomial;