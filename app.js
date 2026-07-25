let chartInstance = null;

document.getElementById('calcBtn').addEventListener('click', calculateAndPlot);

function parsePolynomial(inputStr) {
  if (!inputStr) return [];
  return inputStr.trim().split(/\s+/).map(Number);
}

function evaluatePoly(coeffs, x) {
  let result = 0;
  for (let i = 0; i < coeffs.length; i++) {
    result += coeffs[i] * Math.pow(x, i);
  }
  return result;
}

function getDerivative(coeffs) {
  if (coeffs.length <= 1) return [0];
  let deriv = [];
  for (let i = 1; i < coeffs.length; i++) {
    deriv.push(coeffs[i] * i);
  }
  return deriv;
}

function formatPoly(coeffs) {
  let terms = [];
  for (let i = coeffs.length - 1; i >= 0; i--) {
    let c = coeffs[i];
    if (c === 0) continue;
    let term = "";
    if (Math.abs(c) !== 1 || i === 0) term += Math.abs(c);
    if (i > 0) term += "x";
    if (i > 1) term += "^" + i;
    if (c < 0) term = "- " + term;
    else if (terms.length > 0) term = "+ " + term;
    terms.push(term);
  }
  return terms.length > 0 ? terms.join(" ") : "0";
}

function getTangentLine(coeffs, x0) {
  const d1 = getDerivative(coeffs);
  const m = evaluatePoly(d1, x0);
  const y0 = evaluatePoly(coeffs, x0);
  const c = y0 - m * x0;
  return `y = ${m.toFixed(2)}x ${c >= 0 ? '+ ' + c.toFixed(2) : '- ' + Math.abs(c).toFixed(2)}`;
}

function getNormalLine(coeffs, x0) {
  const d1 = getDerivative(coeffs);
  const derivVal = evaluatePoly(d1, x0);
  if (Math.abs(derivVal) < 1e-7) return "Vertical Line (Undefined Slope)";
  const m = -1 / derivVal;
  const y0 = evaluatePoly(coeffs, x0);
  const c = y0 - m * x0;
  return `y = ${m.toFixed(2)}x ${c >= 0 ? '+ ' + c.toFixed(2) : '- ' + Math.abs(c).toFixed(2)}`;
}

function calculateAndPlot() {
  const input = document.getElementById('coeffs').value;
  const evalX = parseFloat(document.getElementById('evalX').value) || 0;
  const tangentX = parseFloat(document.getElementById('tangentX').value) || 0;

  const coeffs = parsePolynomial(input);

  if (coeffs.length === 0 || coeffs.some(isNaN)) {
    alert("Please enter valid space-separated numbers for coefficients.");
    return;
  }

  const d1 = getDerivative(coeffs);
  const d2 = getDerivative(d1);

  document.getElementById('poly-display').innerText = formatPoly(coeffs);
  document.getElementById('poly-eval').innerText = `P(${evalX}) = ${evaluatePoly(coeffs, evalX)}`;
  document.getElementById('deriv-1').innerText = formatPoly(d1);
  document.getElementById('deriv-2').innerText = formatPoly(d2);
  document.getElementById('tangent-display').innerText = getTangentLine(coeffs, tangentX);
  document.getElementById('normal-display').innerText = getNormalLine(coeffs, tangentX);

  plotChart(coeffs);
}

function plotChart(coeffs) {
  const labels = [];
  const data = [];

  for (let x = -10; x <= 10; x += 0.5) {
    labels.push(x.toFixed(1));
    data.push(evaluatePoly(coeffs, x));
  }

  const ctx = document.getElementById('polyChart').getContext('2d');
  if (chartInstance) chartInstance.destroy();

  chartInstance = new Chart(ctx, {
    type: 'line',
    data: {
      labels: labels,
      datasets: [{
        label: 'P(x)',
        data: data,
        borderColor: '#58a6ff',
        backgroundColor: 'rgba(88, 166, 255, 0.1)',
        fill: true,
        tension: 0.3
      }]
    },
    options: {
      responsive: true,
      scales: {
        x: { title: { display: true, text: 'x', color: '#c9d1d9' }, grid: { color: '#30363d' } },
        y: { title: { display: true, text: 'P(x)', color: '#c9d1d9' }, grid: { color: '#30363d' } }
      }
    }
  });
}