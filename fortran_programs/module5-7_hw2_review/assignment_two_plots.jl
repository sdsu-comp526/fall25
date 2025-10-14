using Plots
default(linewidth=4, legendfontsize=12)

#### Part 4: Conditioning 
f(x) = exp(-x) * ( 3.2 * sin(x) - 0.5 * cos(x) ) - 3.0

plot(f, label = "f(x)")

fprime(x) = -exp(-x)*(3.2*sin(x)- 0.5*cos(x)) + exp(-x)*(3.2*cos(x)+0.5*sin(x))

kappa(x) = abs(fprime(x)) * abs(x) / abs(f(x))

plot(x -> kappa(x), ylims=(0,20), label = "kappa for f(x)")
savefig("kappa.png")

### EC: Convergence rate

hist = zeros(15)

# open output.txt data and store values in the array called hist
open("../fortran_programs/module5-7_hw2_review/output.txt","r") do f
    line = 1
    while ! eof(f)
        l = readline(f)
        hist[line] = parse.(Float64,l)
        line += 1
    end
end

r = hist[end]
scatter( abs.(hist .- r) .+ eps(Float64), yscale=:log10)
ks = 1:length(hist)
plot!(ks, 7 * (.5 .^ ks))
savefig("error_plot.png")

